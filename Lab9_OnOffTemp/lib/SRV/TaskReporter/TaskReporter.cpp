#include "TaskReporter.h"
#include "AppState.h"
#include "LcdDisplay.h"

// LCD page rotates every 3 reporter cycles to give time to read each page.
#define LCD_PAGE_HOLD 3

void TaskReporter_Task(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(10));
    TickType_t xLastWakeTime = xTaskGetTickCount();
    char row0[17], row1[17];
    uint8_t holdCount = 0;
    uint8_t page = 0;

    for (;;) {
        float temp  = AppState_GetTemperature();
        float hum   = AppState_GetHumidity();
        float sp    = AppState_GetSetPoint();
        float hyst  = AppState_GetHysteresis();
        bool  relay = AppState_GetRelay();
        bool  valid = AppState_IsSensorValid();
        float lo    = sp - hyst;
        float hi    = sp + hyst;

        if (++holdCount >= LCD_PAGE_HOLD) {
            holdCount = 0;
            page = (page + 1) % 3;
        }

        // ===== LCD: 3 rotating pages =====
        switch (page) {
        case 0:
            if (valid) snprintf(row0, sizeof(row0), "Now %5.1fC      ", temp);
            else       snprintf(row0, sizeof(row0), "Now ---.--      ");
            snprintf(row1, sizeof(row1), "Goal %.1fC %s",
                     sp, relay ? "HEAT" : "IDLE");
            break;
        case 1:
            snprintf(row0, sizeof(row0), "Band %.1f-%.1f", lo, hi);
            snprintf(row1, sizeof(row1), "Hyst+/-%.2fC   ", hyst);
            break;
        default:  // case 2
            if (valid) snprintf(row0, sizeof(row0), "Hum %4.1f%%       ", hum);
            else       snprintf(row0, sizeof(row0), "Hum ---.-       ");
            snprintf(row1, sizeof(row1), "Heater: %s     ", relay ? "RUNNING" : "STOPPED");
            break;
        }

        LcdDisplay_PrintAt(0, 0, row0);
        LcdDisplay_PrintAt(0, 1, row1);

        // ===== Teleplot output (printf, STDIO) =====
        // One line per variable, `>Name:value` — drag any onto a chart.
        // Relay is scaled to setpoint so it overlays the temperature trace.
        float relayPlot = relay ? sp : 0.0f;
        printf(">Temp:%.2f\n",     valid ? temp : 0.0f);
        printf(">SetPoint:%.2f\n", sp);
        printf(">Lo:%.2f\n",       lo);
        printf(">Hi:%.2f\n",       hi);
        printf(">Relay:%.2f\n",    relayPlot);
        if (valid) printf(">Humidity:%.2f\n", hum);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(DISPLAY_TASK_PERIOD_MS));
    }
}
