#include "TaskReporter.h"
#include "AppState.h"
#include "LcdDisplay.h"

#define FULL_REPORT_EVERY 20  // every 20 x 500ms = 10 seconds

void TaskReporter_Task(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(10));
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint8_t reportCounter = 0;

    char line1[17];
    char line2[17];

    for (;;) {
        CmdState_t cmd   = AppState_GetCommand();
        CmdState_t cond  = AppState_GetConditioned();
        bool       relay = AppState_GetRelayState();
        uint32_t   tgl   = AppState_GetToggleCount();

        const char *cmdStr   = (cmd == CMD_ON)  ? "ON " : "OFF";
        const char *condStr  = (cond == CMD_ON) ? "ON " : "OFF";
        const char *relayStr = relay            ? "ON " : "OFF";

        // ===== LCD output =====
        snprintf(line1, sizeof(line1), "Cmd:%s Rly:%s", cmdStr, relayStr);
        snprintf(line2, sizeof(line2), "Cond:%s  T:%lu", condStr, (unsigned long)tgl);

        LcdDisplay_PrintAt(0, 0, line1);
        LcdDisplay_PrintAt(0, 1, line2);

        // ===== Serial Plotter (printf/STDIO) =====
        printf("Cmd:%d,Conditioned:%d,Relay:%d\n",
               (int)cmd, (int)cond, relay ? 1 : 0);

        // ===== Full report every 10 seconds =====
        reportCounter++;
        if (reportCounter >= FULL_REPORT_EVERY) {
            reportCounter = 0;
            printf("--- REPORT ---\n");
            printf("  Command     : %s\n", cmdStr);
            printf("  Conditioned : %s\n", condStr);
            printf("  Relay       : %s\n", relayStr);
            printf("  Toggles     : %lu\n", (unsigned long)tgl);
            printf("--------------\n");
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(DISPLAY_TASK_PERIOD_MS));
    }
}
