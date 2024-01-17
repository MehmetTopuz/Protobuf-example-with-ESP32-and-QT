#ifndef _APP_TASKS_
#define _APP_TASKS_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TASKS_TO_CREATE     ((size_t)(sizeof(TaskConfigArr) / sizeof(TaskConfigArr[0])))

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
void udpReceiveTask(void *param);


#endif