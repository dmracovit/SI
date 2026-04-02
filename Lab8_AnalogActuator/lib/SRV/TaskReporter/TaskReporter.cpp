#include "TaskReporter.h"
#include "AppState.h"
#include "LcdDisplay.h"

#define FULL_REPORT_EVERY 20

void TaskReporter_Task(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(10));
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint8_t reportCounter = 0;

    char line1[17];
    char line2[17];

    for (;;) {
        bool    relay  = AppState_GetRelayState();
        float   target = AppState_GetTarget();
        float   actual = AppState_GetActual();
        uint8_t alert  = AppState_GetAlert();

        // ===== LCD (16x2) =====
        // Line 1: "Rly:ON  Srv:090"
        snprintf(line1, sizeof(line1), "Rly:%s Srv:%03d",
                 relay ? "ON " : "OFF", (int)actual);

        // Line 2: "Tgt:090  STABLE"
        const char *status;
        if (alert & ALERT_RAMPING)       status = "RAMP";
        else if (alert & ALERT_LIMIT_HI) status = "HI! ";
        else if (alert & ALERT_LIMIT_LO) status = "LO! ";
        else                             status = " OK ";

        snprintf(line2, sizeof(line2), "Tgt:%03d  [%s]", (int)target, status);

        LcdDisplay_PrintAt(0, 0, line1);
        LcdDisplay_PrintAt(0, 1, line2);

        // ===== Serial Plotter =====
        printf("Relay:%d,Target:%.1f,Actual:%.1f\n",
               relay ? 1 : 0, target, actual);

        // ===== Full report =====
        reportCounter++;
        if (reportCounter >= FULL_REPORT_EVERY) {
            reportCounter = 0;
            printf("--- REPORT ---\n");
            printf("  Relay  : %s (binary actuator)\n", relay ? "ON" : "OFF");
            printf("  Target : %.1f deg\n", target);
            printf("  Actual : %.1f deg\n", actual);
            printf("  Alert  : %s%s%s%s\n",
                   (alert == ALERT_NONE) ? "NONE" : "",
                   (alert & ALERT_LIMIT_LO) ? "LO " : "",
                   (alert & ALERT_LIMIT_HI) ? "HI " : "",
                   (alert & ALERT_RAMPING)  ? "RAMPING " : "");
            printf("--------------\n");
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(DISPLAY_TASK_PERIOD_MS));
    }
}
