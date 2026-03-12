#include "TaskReporter.h"
#include "SensorData.h"
#include "LcdDisplay.h"

#define REPORTER_PERIOD_MS 500
#define FULL_REPORT_EVERY  10   // print full text report every N cycles (10 x 500ms = 5s)

void TaskReporter_Task(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(10));  // small offset
    TickType_t xLastWakeTime = xTaskGetTickCount();

    char line1[17];  // LCD line buffer (16 chars + null)
    char line2[17];
    uint8_t reportCounter = 0;

    for (;;) {
        float        temp   = SensorData_GetTemperature();
        bool         tValid = SensorData_IsTempValid();
        AlertState_t tAlert = SensorData_GetTempAlert();
        uint8_t      tDbg   = SensorData_GetTempDebounce();

        int          snd    = SensorData_GetSoundLevel();
        AlertState_t sAlert = SensorData_GetSoundAlert();
        uint8_t      sDbg   = SensorData_GetSoundDebounce();

        const char *tState = (tAlert == ALERT_ON) ? "ALRT" : " OK ";
        const char *sState = (sAlert == ALERT_ON) ? "ALRT" : " OK ";

        // ===== LCD output (16x2) =====
        if (tValid) {
            snprintf(line1, sizeof(line1), "T:%5.1fC S:%-4d", temp, snd);
        } else {
            snprintf(line1, sizeof(line1), "T: ERR   S:%-4d", snd);
        }
        snprintf(line2, sizeof(line2), "T:%s    S:%s", tState, sState);

        LcdDisplay_PrintAt(0, 0, line1);
        LcdDisplay_PrintAt(0, 1, line2);

        // ===== Serial output via printf (STDIO) =====
        // Plotter format every 500ms
        printf("Temp:%.1f,TempHi:%.1f,TempLo:%.1f,Snd:%d,SndHi:%d,SndLo:%d\n",
               tValid ? temp : 0.0f,
               TEMP_THRESH_HIGH, TEMP_THRESH_LOW,
               snd, SOUND_THRESH_HIGH, SOUND_THRESH_LOW);

        // Structured text report every 5 seconds (via printf = STDIO)
        reportCounter++;
        if (reportCounter >= FULL_REPORT_EVERY) {
            reportCounter = 0;
            printf("--- REPORT ---\n");
            printf("  Temp : %.1f C [%s] debounce:%d/%d\n",
                   tValid ? temp : -1.0f, tState, tDbg, DEBOUNCE_COUNT);
            printf("  Sound: %d    [%s] debounce:%d/%d\n",
                   snd, sState, sDbg, DEBOUNCE_COUNT);
            printf("--------------\n");
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(REPORTER_PERIOD_MS));
    }
}
