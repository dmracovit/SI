#include "TaskDisplay.h"
#include "AppState.h"
#include "FsmCore.h"
#include "LcdDisplay.h"
#include <stdio.h>

// Renders two lines:
//   Line 0: "St: <state name>"
//   Line 1: "Trans: <count>"

void TaskDisplay_Task(void *pvParameters)
{
    (void)pvParameters;
    vTaskDelay(pdMS_TO_TICKS(20));
    TickType_t cycleStart = xTaskGetTickCount();

    char row0[17];
    char row1[17];

    for (;;) {
        const FsmState_t st = FsmCore_GetState();
        const uint32_t    n = AppState_GetTransitions();

        snprintf(row0, sizeof(row0), "St: %-12s", FsmCore_StateName(st));
        snprintf(row1, sizeof(row1), "Trans: %-8lu",  (unsigned long)n);

        // LCD access is shared between this task and potential debug prints;
        // serialise it via the mutex held in AppState.
        if (xSemaphoreTake(xLcdMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            LcdDisplay_PrintAt(0, 0, row0);
            LcdDisplay_PrintAt(0, 1, row1);
            xSemaphoreGive(xLcdMutex);
        }

        // Teleplot-compatible serial trace of the state index.
        printf(">State:%d\n",       (int)st);
        printf(">Transitions:%lu\n", (unsigned long)n);

        vTaskDelayUntil(&cycleStart, pdMS_TO_TICKS(DISPLAY_PERIOD_MS));
    }
}
