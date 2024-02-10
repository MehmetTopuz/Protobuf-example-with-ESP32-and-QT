#ifndef _APP_TASKS_
#define _APP_TASKS_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hydroponic_data.pb.h"
#include "pb.h"
#include "pb_common.h"
#include "pb_decode.h"
#include "pb_encode.h"


#define TASKS_TO_CREATE     ((size_t)(sizeof(TaskConfigArr) / sizeof(TaskConfigArr[0])))

#define SERVER_IP           "192.168.38.188"
#define SERVER_PORT         5000


typedef struct {
    TaskFunction_t TaskFunc;
    const char * const TaskName;
    uint16_t StackDepth;
    void * TaskParameter;
    UBaseType_t TaskPriority;
    TaskHandle_t *TaskHandle;
}TaskConfig_t;

void initalizeTasks(void);
void udpClientTask(void *param);
int serializeData(uint8_t *buffer, size_t len, hydroponic_Hydroponic *message);
int deSerializeData(uint8_t *buffer, size_t len);
void generateSampleHydroponicData(hydroponic_Hydroponic *message);

#endif