#include "TaskReporter.h"
#include "Signals.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void TaskReporter_Run(void)
{
    uint32_t total    = Signals_GetTotalPresses();
    uint32_t shorts   = Signals_GetShortPresses();
    uint32_t longs    = Signals_GetLongPresses();
    uint32_t shortDur = Signals_GetTotalShortDuration();
    uint32_t longDur  = Signals_GetTotalLongDuration();

    uint32_t avgShort = (shorts > 0) ? (shortDur / shorts) : 0;
    uint32_t avgLong  = (longs  > 0) ? (longDur  / longs)  : 0;

    printf("\n--- Report (last 10s) ---\n");
    printf("Total  : %lu\n", total);
    printf("Short  : %lu  avg %lu ms\n", shorts, avgShort);
    printf("Long   : %lu  avg %lu ms\n", longs,  avgLong);
    printf("-------------------------\n");

    Signals_ResetAll();
}

void TaskReporter_Task(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(10));
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        TaskReporter_Run();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10000));
    }
}
