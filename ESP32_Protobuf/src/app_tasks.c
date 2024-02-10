#include <string.h>
#include <sys/param.h>
#include "app_tasks.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_system.h" 
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include <lwip/netdb.h>

#include "hydroponic_data.pb.h"
#include "pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"

static const char *TAG = "ESP32";

// Task configuration table: Add task properties to this array when you want to create a new task.

TaskConfig_t TaskConfigArr[] = {
    {(TaskFunction_t)udpClientTask, "UDP Client Task", 4096, NULL, 0, NULL}
};


void initalizeTasks(void){

    BaseType_t retVal = pdPASS;

    for(int i=0; i<TASKS_TO_CREATE; i++){

        retVal = xTaskCreate(TaskConfigArr[i].TaskFunc,
                            TaskConfigArr[i].TaskName,
                            TaskConfigArr[i].StackDepth,
                            TaskConfigArr[i].TaskParameter,
                            TaskConfigArr[i].TaskPriority,
                            TaskConfigArr[i].TaskHandle);

        if(retVal == pdFAIL){
            ESP_LOGI(TAG, "Task creation failed.<%s>", TaskConfigArr[i].TaskName);
        }
    }

}

void udpClientTask(void *param){

    int addr_family = 0;
    int ip_protocol = 0;
    uint8_t buffer[128] = {0};
    hydroponic_Hydroponic messageToSend = hydroponic_Hydroponic_init_default;

    while (1) {
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(SERVER_PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        ESP_LOGI(TAG, "Socket created, sending to %s:%d", SERVER_IP, SERVER_PORT);

        while (1) {

            generateSampleHydroponicData(&messageToSend);

            int size = serializeData(buffer, sizeof(buffer), &messageToSend);
            ESP_LOGI(TAG, "Size: %d", size);
            int err = sendto(sock, buffer, size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                break;
            }
            ESP_LOGI(TAG, "Message sent");

            // Decode the serialized data to verify that encoding is successful.
            //deSerializeData(buffer,size);
            vTaskDelay(pdMS_TO_TICKS(1000));


        }

        if (sock != -1) {
        ESP_LOGE(TAG, "Shutting down socket and restarting...");
        shutdown(sock, 0);
        close(sock);
        }
    }
    vTaskDelete(NULL);
}

bool write_string(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg)
{
    if (!pb_encode_tag_for_field(stream, field))
        return false;

    return pb_encode_string(stream, (uint8_t*)*arg, strlen((char*)*arg));
}

bool read_string(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    uint8_t buffer[128] = {0};
    
    /* We could read block-by-block to avoid the large buffer... */
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;
    /* Print the string, in format comparable with protoc --decode.
     * Format comes from the arg defined in main().
     */
    //printf((char*)*arg, buffer); 
    strcpy((char*)*arg, (char*)buffer);
    return true;
}

bool msg_callback(pb_istream_t *stream, const pb_field_t *field, void **arg)
{

    // hydroponic_Hydroponic *topmsg = field->message;
    // ESP_LOGI(TAG,"Message Type: %d" , (int)topmsg->messageType);

    if (field->tag == hydroponic_Hydroponic_dataPackage_tag)
    {


        hydroponic_DataPackage *message = field->pData;

        message->sector.funcs.decode =& read_string;
        message->sector.arg = malloc(10*sizeof(char));

    }

    else if (field->tag == hydroponic_Hydroponic_messageOk_tag)
    {
        hydroponic_MessageOk *message = field->pData;

        message->responseMessage.funcs.decode =& read_string;
        message->responseMessage.arg = malloc(50*sizeof(char));
       
    }

    else if (field->tag == hydroponic_Hydroponic_messageError_tag)
    {

        hydroponic_MessageError *message = field->pData;

        message->errorType.funcs.decode =& read_string;
        message->errorType.arg = malloc(50*sizeof(char));

    }

    else if (field->tag == hydroponic_Hydroponic_messageTimeout_tag)
    {
        hydroponic_MessageTimeout *message = field->pData;

        message->timeoutMessage.funcs.decode =& read_string;
        message->timeoutMessage.arg = malloc(50*sizeof(char));
      
    }

    return true;
}

int serializeData(uint8_t *buffer, size_t len, hydroponic_Hydroponic *message){

    pb_ostream_t ostream = pb_ostream_from_buffer(buffer, len);

    pb_encode(&ostream, hydroponic_Hydroponic_fields, message);

    return ostream.bytes_written;
    
}

 int deSerializeData(uint8_t *buffer, size_t len){

    hydroponic_Hydroponic receivedMessage = hydroponic_Hydroponic_init_zero;
    pb_istream_t istream = pb_istream_from_buffer(buffer, len);
    // receivedMessage.sector.funcs.decode =& read_string;
    // receivedMessage.sector.arg = malloc(10*sizeof(char));

    receivedMessage.cb_msg.funcs.decode = msg_callback;

    bool ret = pb_decode(&istream, hydroponic_Hydroponic_fields, &receivedMessage); 

    if(!ret){
        ESP_LOGI(TAG, "Decode Error.");
        return 0;

    }

    if(receivedMessage.which_msg == hydroponic_Hydroponic_dataPackage_tag){

        ESP_LOGI(TAG, "Data Package Received.");

        ESP_LOGI(TAG, "Device ID: %ld", receivedMessage.msg.dataPackage.deviceID);
        ESP_LOGI(TAG, "Sector: %s", (char*)receivedMessage.msg.dataPackage.sector.arg); 
        ESP_LOGI(TAG, "E conductivity: %f", receivedMessage.msg.dataPackage.eConductivity); 
        ESP_LOGI(TAG, "Ph: %f", receivedMessage.msg.dataPackage.ph); 
        ESP_LOGI(TAG, "Moisture: %f", receivedMessage.msg.dataPackage.moisture); 
        ESP_LOGI(TAG, "Temperature: %f", receivedMessage.msg.dataPackage.temperature); 
        ESP_LOGI(TAG, "Water Level: %ld", receivedMessage.msg.dataPackage.waterLevel); 
        ESP_LOGI(TAG, "Valve State: %d", receivedMessage.msg.dataPackage.valveState); 
        ESP_LOGI(TAG, "Pump State: %d", receivedMessage.msg.dataPackage.pumpState); 
        ESP_LOGI(TAG, "Led State: %d", receivedMessage.msg.dataPackage.ledStatus); 

        //delete arg
        free(receivedMessage.msg.dataPackage.sector.arg);

    }
    else if(receivedMessage.which_msg == hydroponic_Hydroponic_heartBeat_tag){
        ESP_LOGI(TAG, "Heartbeat Package Received.");
        ESP_LOGI(TAG, "Elapsed time %ld.", receivedMessage.msg.heartBeat.elapsedTime);

    }
    else if(receivedMessage.which_msg == hydroponic_Hydroponic_messageOk_tag){
        ESP_LOGI(TAG, "Message OK Package Received.");
        ESP_LOGI(TAG, "%s", (char*)receivedMessage.msg.messageOk.responseMessage.arg);

        free(receivedMessage.msg.messageOk.responseMessage.arg);

    }
    else if(receivedMessage.which_msg == hydroponic_Hydroponic_messageError_tag){
        ESP_LOGI(TAG, "Message Error Package Received.");
        ESP_LOGI(TAG, "%s", (char*)receivedMessage.msg.messageError.errorType.arg);

        free(receivedMessage.msg.messageError.errorType.arg);

    }
    else if(receivedMessage.which_msg == hydroponic_Hydroponic_messageTimeout_tag){
        ESP_LOGI(TAG, "Message timeout Package Received.");
        ESP_LOGI(TAG, "%s", (char*)receivedMessage.msg.messageTimeout.timeoutMessage.arg);

        free(receivedMessage.msg.messageTimeout.timeoutMessage.arg);

    }

    return 1;
}

void generateSampleHydroponicData(hydroponic_Hydroponic *message){

    message->messageType = hydroponic_MessageType_MSG_DATA;

    message->which_msg = hydroponic_Hydroponic_dataPackage_tag;  // Deciding which message will be sent.

    message->msg.dataPackage.deviceID = 10;
    message->msg.dataPackage.sector.arg = "Sector-1";
    message->msg.dataPackage.sector.funcs.encode =& write_string;
    
    if(message->msg.dataPackage.eConductivity >= 20.0f) message->msg.dataPackage.eConductivity = 0.0f; else message->msg.dataPackage.eConductivity += 1.0f;
    if(message->msg.dataPackage.ph >= 14.0f) message->msg.dataPackage.ph = 0.0f; else message->msg.dataPackage.ph += 0.1f;
    if(message->msg.dataPackage.moisture >= 50.0f) message->msg.dataPackage.moisture = 10.0f; else message->msg.dataPackage.moisture += 1.0f;
    if(message->msg.dataPackage.temperature >= 30.0f) message->msg.dataPackage.temperature = 10.0f; else message->msg.dataPackage.temperature += 1.0f;
    if(message->msg.dataPackage.waterLevel >= 100) message->msg.dataPackage.waterLevel = 0; else message->msg.dataPackage.waterLevel += 5;
    //if(message->msg.dataPackage.waterLevel >= 100) message->msg.dataPackage.valveState = false; else message->msg.dataPackage.valveState = true;

    message->msg.dataPackage.valveState = !message->msg.dataPackage.valveState;
    message->msg.dataPackage.pumpState = !message->msg.dataPackage.pumpState;
    message->msg.dataPackage.ledStatus = !message->msg.dataPackage.ledStatus;

// sample HeartBeat Message
    // message->messageType = hydroponic_MessageType_MSG_HEART_BEAT;
    // message->which_msg = hydroponic_Hydroponic_heartBeat_tag;
    // message->msg.heartBeat.elapsedTime = 45;

// sample Message Ok

    // message->messageType = hydroponic_MessageType_MSG_OK;
    // message->which_msg = hydroponic_Hydroponic_messageOk_tag;
    // message->msg.messageOk.responseMessage.arg = "Message Accepted.";
    // message->msg.messageOk.responseMessage.funcs.encode =& write_string;
    
// sample Message Error

    // message->messageType = hydroponic_MessageType_MSG_ERROR;
    // message->which_msg = hydroponic_Hydroponic_messageError_tag;
    // message->msg.messageError.errorType.arg = "Error occured.";
    // message->msg.messageError.errorType.funcs.encode =& write_string;

// sample Message Timeout

    // message->messageType = hydroponic_MessageType_MSG_TIMEOUT;
    // message->which_msg = hydroponic_Hydroponic_messageTimeout_tag;
    // message->msg.messageTimeout.timeoutMessage.arg = "Timeout occured.";
    // message->msg.messageTimeout.timeoutMessage.funcs.encode =& write_string;

}
