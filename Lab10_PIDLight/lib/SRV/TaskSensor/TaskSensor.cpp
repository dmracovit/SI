#include "TaskSensor.h"
#include "AppState.h"
#include "LdrSensor.h"

// Reads LDR, applies EMA filter, stores in AppState.
void TaskSensor_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    float emaValue = LdrSensor_ReadPercent();  // seed with first reading

    for (;;) {
        float raw = LdrSensor_ReadPercent();
        emaValue = LDR_EMA_ALPHA * raw + (1.0f - LDR_EMA_ALPHA) * emaValue;
        AppState_SetLight(emaValue);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SENSOR_TASK_PERIOD_MS));
    }
}
