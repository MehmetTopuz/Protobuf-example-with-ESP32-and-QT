#include <string.h>
#include <sys/param.h>
#include "app_tasks.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
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
#include "protobuf_callbacks.h"
#include "pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"

static const char *TAG = "ESP32";
static int udpSocket = -1;

static QueueHandle_t commandQueue;

//Temporary variables for testing purposes.

bool ledState = false, valveState = false, pumpState = false;

// Task configuration table: Add task properties to this array when you want to create a new task.

TaskConfig_t TaskConfigArr[] = {
    {(TaskFunction_t)udpSenderTask, "UDP Sender Task", 4096, &udpSocket, 0, NULL},
    {(TaskFunction_t)udpReceiverTask, "UDP Receiver Task", 4096, &udpSocket, 0, NULL},
    {(TaskFunction_t)commandProcessTask, "Command Task", 4096, NULL, 1, NULL}
};

static int createSocket(){

    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_IP;

    int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
    if (sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
    }

        // Set timeout
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

    ESP_LOGI(TAG, "Socket created");

    return sock;

}

void initalizeTasks(void){

    BaseType_t retVal = pdPASS;

    commandQueue = xQueueCreate(10, sizeof(hydroponic_CMD));

    udpSocket = createSocket(); // create a global socket variable

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

void udpSenderTask(void *param){

    uint8_t buffer[128] = {0};
    hydroponic_Hydroponic messageToSend = hydroponic_Hydroponic_init_default;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(SERVER_PORT);

    while (1)
    {
        generateSampleHydroponicData(&messageToSend);

        int size = serializeData(buffer, sizeof(buffer), &messageToSend);
        ESP_LOGI(TAG, "Size: %d", size);
        int err = sendto(*(int*)param, buffer, size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err < 0) {
            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "Message sent");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}
void udpReceiverTask(void *param){

    uint8_t buffer[128] = {0};
    hydroponic_Hydroponic receivedMessage = hydroponic_Hydroponic_init_zero;

    struct sockaddr_storage source_addr;
    socklen_t socklen = sizeof(source_addr);

    while (1)
    {
        int len = recvfrom(*(int*)param, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);
        
        // Error occurred during receiving
        if (len < 0) {
            ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
        }
        // Data received
        else {
            
            ESP_LOGI(TAG, "Received %d bytes", len);
            ESP_LOGI(TAG, "%s", buffer);
            
            // decode protobuf message
            deSerializeData(buffer,len, &receivedMessage);

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
            else if(receivedMessage.which_msg == hydroponic_Hydroponic_cmd_tag){
                //ESP_LOGI(TAG, "Command Package Received.");
                
                //ESP_LOGI(TAG, "Received cmd: %d", receivedMessage.msg.cmd.command);
                // add to command queue
                xQueueSendToBack(commandQueue, &receivedMessage.msg.cmd.command, 0);
            }

        }
    }
    
}

void commandProcessTask(void *param){

    BaseType_t retVal = pdFALSE;
    hydroponic_CMD receivedCommand; 

    while(1){
        retVal = xQueueReceive(commandQueue, &receivedCommand, portMAX_DELAY);

        if(retVal == pdPASS){
            //ESP_LOGI(TAG, "Command received: %d", receivedCommand);

            // handle commands
            switch (receivedCommand)
            {
            case hydroponic_CMD_CMD_VALVE_ON:
                valveState = true;
                // do stuff
                break;
            
            case hydroponic_CMD_CMD_VALVE_OFF:
                valveState = false;
                break;

            case hydroponic_CMD_CMD_PUMP_ON:
                pumpState = true;
                break;

            case hydroponic_CMD_CMD_PUMP_OFF:
                pumpState = false;
                break;

            case hydroponic_CMD_CMD_LED_ON:
                ledState = true;
                break;

            case hydroponic_CMD_CMD_LED_OFF:
                ledState = false;
                break;
            
            default:
                break;
            }
        }
        else{
            ESP_LOGI(TAG, "Command queue reached to portMAX_DELAY");
        }
    }
}

int serializeData(uint8_t *buffer, size_t len, hydroponic_Hydroponic *message){

    pb_ostream_t ostream = pb_ostream_from_buffer(buffer, len);

    pb_encode(&ostream, hydroponic_Hydroponic_fields, message);

    return ostream.bytes_written;
    
}

int deSerializeData(uint8_t *buffer, size_t len, hydroponic_Hydroponic *message){

    pb_istream_t istream = pb_istream_from_buffer(buffer, len);

    message->cb_msg.funcs.decode = msg_callback;

    bool ret = pb_decode(&istream, hydroponic_Hydroponic_fields, message); 

    if(!ret){
        ESP_LOGI(TAG, "Decode Error.");
        return 0;

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

    message->msg.dataPackage.valveState = valveState;
    message->msg.dataPackage.pumpState = pumpState;
    message->msg.dataPackage.ledStatus = ledState;

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
