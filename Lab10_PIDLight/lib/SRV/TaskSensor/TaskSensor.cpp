#include "TaskSensor.h"
#include "AppState.h"
#include "LdrSensor.h"

// Sensor task: read the photoresistor, smooth it, hand the result to the
// controller via AppState.
//
// The EMA is written in its incremental form
//
//      filtered  ←  filtered + α · (sample - filtered)
//
// which is algebraically identical to  α·sample + (1-α)·filtered  but
// requires only one multiplication and avoids accumulating round-off
// error from the (1-α) factor.

void TaskSensor_Task(void *pvParameters)
{
    (void)pvParameters;

    // Seed the filter with the first reading so the controller does not
    // see a 0 → real_value step on the very first cycle.
    float smoothed = LdrSensor_ReadPercent();
    AppState_SetLight(smoothed);

    TickType_t cycleStart = xTaskGetTickCount();

    for (;;)
    {
        const float fresh = LdrSensor_ReadPercent();
        smoothed += LDR_EMA_ALPHA * (fresh - smoothed);
        AppState_SetLight(smoothed);

        vTaskDelayUntil(&cycleStart, pdMS_TO_TICKS(SENSOR_TASK_PERIOD_MS));
    }
}
