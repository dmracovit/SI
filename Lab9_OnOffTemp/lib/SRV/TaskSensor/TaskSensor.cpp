#include "TaskSensor.h"
#include "AppState.h"
#include "DhtSensor.h"

void TaskSensor_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        float temp, hum;
        bool valid = DhtSensor_Read(&temp, &hum);
        if (valid) AppState_SetSensor(temp, hum, true);
        else       AppState_SetSensor(0, 0, false);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SENSOR_TASK_PERIOD_MS));
    }
}
