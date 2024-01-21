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
static const char *payload = "Message from ESP32 ";

TaskConfig_t TaskConfigArr[] = {
    {(TaskFunction_t)udpClientTask, "UDP Client Task", 4096, NULL, 0, NULL},
    {(TaskFunction_t)udpReceiveTask, "UDP Receive Task", 4096, NULL, 0, NULL}
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

            int size = serializeData(buffer, sizeof(buffer));
            int err = sendto(sock, buffer, size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                break;
            }
            ESP_LOGI(TAG, "Message sent");

            deSerializeData(buffer, size);
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

void udpReceiveTask(void *param){

    
    while(1){


    }
}
bool write_string(pb_ostream_t *stream, const pb_field_iter_t *field, void * const *arg)
{
    if (!pb_encode_tag_for_field(stream, field))
        return false;

    return pb_encode_string(stream, (uint8_t*)*arg, strlen((char*)*arg));
}

bool read_string(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    uint8_t buffer[20] = {0};
    
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


int serializeData(uint8_t *buffer, size_t len){

    hydroponicData_hDataPacket message = hydroponicData_hDataPacket_init_zero;

    message.messageType = hydroponicData_MessageType_MSG_DATA;
    message.deviceID = 1;
    message.sector.arg = "Sector-1";
    message.sector.funcs.encode =& write_string;

    pb_ostream_t ostream = pb_ostream_from_buffer(buffer, len);

    pb_encode(&ostream, hydroponicData_hDataPacket_fields, &message);

    return ostream.bytes_written;
}

int deSerializeData(uint8_t *buffer, size_t len){

    hydroponicData_hDataPacket receivedMessage =hydroponicData_hDataPacket_init_zero;
    pb_istream_t istream = pb_istream_from_buffer(buffer, len);
    receivedMessage.sector.funcs.decode =& read_string;
    receivedMessage.sector.arg = malloc(10*sizeof(char));
    pb_decode(&istream, hydroponicData_hDataPacket_fields, &receivedMessage); 

    ESP_LOGI(TAG, "Decoded message: %d", receivedMessage.messageType);
    ESP_LOGI(TAG, "Decoded message: %ld", receivedMessage.deviceID);
    ESP_LOGI(TAG, "Decoded message: %s", (char*)receivedMessage.sector.arg); // burada program çöküyor.
    return 1;
}