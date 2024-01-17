#include "app_tasks.h"
#include "esp_log.h"

static const char *TAG = "ESP32";

TaskConfig_t TaskConfigArr[] = {
    {(TaskFunction_t)udpClientTask, "UDP Client Task", 2048, NULL, 0, NULL},
    {(TaskFunction_t)udpReceiveTask, "UDP Client Task", 2048, NULL, 0, NULL}
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


    while(1){


    }
}

void udpReceiveTask(void *param){

    
    while(1){


    }
}