#include "TaskReporter.h"
#include "SensorData.h"
#include "LcdDisplay.h"

#define REPORTER_PERIOD_MS 500
#define FULL_REPORT_EVERY 10 // full text report every 10 x 500ms = 5s

void TaskReporter_Task(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(10)); // small offset
    TickType_t xLastWakeTime = xTaskGetTickCount();

    char line1[17];
    char line2[17];
    uint8_t reportCounter = 0;

    for (;;)
    {
        // --- Read all data ---
        float rawTemp     = SensorData_GetTemperature();
        bool  tValid      = SensorData_IsTempValid();
        float filtTemp    = SensorData_GetTempFiltered();
        AlertState_t tAl  = SensorData_GetTempAlert();

        int   rawSnd      = SensorData_GetSoundLevel();
        float filtSnd     = SensorData_GetSoundFiltered();
        AlertState_t sAl  = SensorData_GetSoundAlert();

        const char *tState = (tAl == ALERT_ON) ? "ALRT" : " OK ";
        const char *sState = (sAl == ALERT_ON) ? "ALRT" : " OK ";

        // ===== LCD output (16x2) =====
        // Line 1: filtered values
        if (tValid)
            snprintf(line1, sizeof(line1), "T:%5.1fC S:%-4d", filtTemp, (int)filtSnd);
        else
            snprintf(line1, sizeof(line1), "T: ERR   S:%-4d", (int)filtSnd);

        // Line 2: alert states
        snprintf(line2, sizeof(line2), "T:%s    S:%s", tState, sState);

        LcdDisplay_PrintAt(0, 0, line1);
        LcdDisplay_PrintAt(0, 1, line2);

        // ===== Serial Plotter format (every 500ms) =====
        printf("TRaw:%.1f,TFilt:%.1f,THi:%.1f,TLo:%.1f,SRaw:%d,SFilt:%.0f,SHi:%d,SLo:%d\n",
               tValid ? rawTemp : 0.0f, filtTemp,
               TEMP_THRESH_HIGH, TEMP_THRESH_LOW,
               rawSnd, filtSnd,
               SOUND_THRESH_HIGH, SOUND_THRESH_LOW);

        // ===== Structured report every 5 seconds =====
        reportCounter++;
        if (reportCounter >= FULL_REPORT_EVERY)
        {
            reportCounter = 0;
            printf("--- REPORT ---\n");
            printf("  Temp  : raw=%.1fC  filt=%.1fC  [%s]\n",
                   tValid ? rawTemp : -1.0f, filtTemp, tState);
            printf("  Sound : raw=%d  filt=%.0f  [%s]\n",
                   rawSnd, filtSnd, sState);
            printf("--------------\n");
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(REPORTER_PERIOD_MS));
    }
}
