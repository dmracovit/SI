#include "TaskConditioning.h"
#include "SensorData.h"
#include "LedDriver.h"

// Conditioning is semaphore-driven: runs immediately when Acquisition signals new data.

// Helper: apply hysteresis + debounce for one sensor channel.
// Returns the confirmed alert state after processing.
static AlertState_t ProcessChannel(
    float value, float threshHigh, float threshLow,
    AlertState_t currentAlert, uint8_t *debounce)
{
    // --- Hysteresis ---
    AlertState_t desired;
    if      (value >= threshHigh) desired = ALERT_ON;
    else if (value <= threshLow)  desired = ALERT_OFF;
    else                          desired = currentAlert;

    // --- Debounce ---
    if (desired != currentAlert) {
        (*debounce)++;
        if (*debounce >= DEBOUNCE_COUNT) {
            *debounce = 0;
            return desired;  // transition confirmed
        }
    } else {
        *debounce = 0;
    }
    return currentAlert;  // no change yet
}

void TaskConditioning_Task(void *pvParameters)
{
    for (;;) {
        // Block until Acquisition signals new data (timeout = 200ms as watchdog)
        if (xSemaphoreTake(xSemNewData, pdMS_TO_TICKS(200)) != pdTRUE) {
            continue;
        }

        // ===== Temperature channel (digital) =====
        if (SensorData_IsTempValid()) {
            float        temp    = SensorData_GetTemperature();
            AlertState_t tAlert  = SensorData_GetTempAlert();
            uint8_t      tDbg    = SensorData_GetTempDebounce();

            AlertState_t newTAlert = ProcessChannel(
                temp, TEMP_THRESH_HIGH, TEMP_THRESH_LOW, tAlert, &tDbg);

            SensorData_SetTempDebounce(tDbg);
            if (newTAlert != tAlert) {
                SensorData_SetTempAlert(newTAlert);
            }
        }

        // ===== Sound channel (analog) =====
        {
            int          snd     = SensorData_GetSoundLevel();
            AlertState_t sAlert  = SensorData_GetSoundAlert();
            uint8_t      sDbg    = SensorData_GetSoundDebounce();

            AlertState_t newSAlert = ProcessChannel(
                (float)snd, (float)SOUND_THRESH_HIGH, (float)SOUND_THRESH_LOW,
                sAlert, &sDbg);

            SensorData_SetSoundDebounce(sDbg);
            if (newSAlert != sAlert) {
                SensorData_SetSoundAlert(newSAlert);
            }
        }

        // ===== Apply LEDs =====
        // Green = both sensors OK, Red = any sensor in alert
        AlertState_t ta = SensorData_GetTempAlert();
        AlertState_t sa = SensorData_GetSoundAlert();

        if (ta == ALERT_ON || sa == ALERT_ON) {
            LedDriver_GreenOff();
            LedDriver_RedOn();
        } else {
            LedDriver_RedOff();
            LedDriver_GreenOn();
        }
    }
}
