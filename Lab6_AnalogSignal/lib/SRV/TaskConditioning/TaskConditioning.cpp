#include "TaskConditioning.h"
#include "SensorData.h"
#include "SignalFilter.h"
#include "LedDriver.h"

// Filter states — persistent across calls (one per sensor channel)
static FilterState_t tempFilter;
static FilterState_t soundFilter;

// Helper: apply hysteresis + debounce for one sensor channel.
static AlertState_t ProcessChannel(
    float value, float threshHigh, float threshLow,
    AlertState_t currentAlert, uint8_t *debounce)
{
    // --- Hysteresis ---
    AlertState_t desired;
    if (value >= threshHigh)
        desired = ALERT_ON;
    else if (value <= threshLow)
        desired = ALERT_OFF;
    else
        desired = currentAlert;

    // --- Debounce ---
    if (desired != currentAlert)
    {
        (*debounce)++;
        if (*debounce >= DEBOUNCE_COUNT)
        {
            *debounce = 0;
            return desired; // transition confirmed
        }
    }
    else
    {
        *debounce = 0;
    }
    return currentAlert; // no change yet
}

void TaskConditioning_Task(void *pvParameters)
{
    SignalFilter_Init(&tempFilter);
    SignalFilter_Init(&soundFilter);

    for (;;)
    {
        // Block until Acquisition signals new data
        if (xSemaphoreTake(xSemNewData, pdMS_TO_TICKS(200)) != pdTRUE)
        {
            continue;
        }

        // ===== Temperature channel (digital) =====
        if (SensorData_IsTempValid())
        {
            float rawTemp = SensorData_GetTemperature();

            // Signal conditioning pipeline: saturate → median → EMA
            float filtered = SignalFilter_Process(
                &tempFilter, rawTemp, TEMP_SAT_MIN, TEMP_SAT_MAX);
            SensorData_SetTempFiltered(filtered);

            // Threshold alerting uses FILTERED value
            AlertState_t tAlert = SensorData_GetTempAlert();
            uint8_t tDbg = SensorData_GetTempDebounce();

            AlertState_t newTAlert = ProcessChannel(
                filtered, TEMP_THRESH_HIGH, TEMP_THRESH_LOW, tAlert, &tDbg);

            SensorData_SetTempDebounce(tDbg);
            if (newTAlert != tAlert)
            {
                SensorData_SetTempAlert(newTAlert);
            }
        }

        // ===== Sound channel (analog) =====
        {
            int rawSnd = SensorData_GetSoundLevel();

            // Signal conditioning pipeline: saturate → median → EMA
            float filtered = SignalFilter_Process(
                &soundFilter, (float)rawSnd, (float)SOUND_SAT_MIN, (float)SOUND_SAT_MAX);
            SensorData_SetSoundFiltered(filtered);

            // Threshold alerting uses FILTERED value
            AlertState_t sAlert = SensorData_GetSoundAlert();
            uint8_t sDbg = SensorData_GetSoundDebounce();

            AlertState_t newSAlert = ProcessChannel(
                filtered, (float)SOUND_THRESH_HIGH, (float)SOUND_THRESH_LOW,
                sAlert, &sDbg);

            SensorData_SetSoundDebounce(sDbg);
            if (newSAlert != sAlert)
            {
                SensorData_SetSoundAlert(newSAlert);
            }
        }

        // ===== Apply LEDs =====
        AlertState_t ta = SensorData_GetTempAlert();
        AlertState_t sa = SensorData_GetSoundAlert();

        if (ta == ALERT_ON || sa == ALERT_ON)
        {
            LedDriver_GreenOff();
            LedDriver_RedOn();
        }
        else
        {
            LedDriver_RedOff();
            LedDriver_GreenOn();
        }
    }
}
