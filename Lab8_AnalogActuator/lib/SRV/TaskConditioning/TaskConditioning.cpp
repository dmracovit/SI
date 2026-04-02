#include "TaskConditioning.h"
#include "AppState.h"
#include "SignalFilter.h"
#include "SignalConditioner.h"
#include "ServoDriver.h"
#include "RelayDriver.h"
#include "LedDriver.h"

static FilterState_t servoFilter;
static CondState_t   relayCond;

void TaskConditioning_Task(void *pvParameters)
{
    SignalFilter_Init(&servoFilter);
    SignalConditioner_Init(&relayCond);

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        // ========== BINARY ACTUATOR: Relay ==========
        bool relayCmd = AppState_GetRelayCmd();

        // Debounce + validation (from Lab7)
        int validated = SignalConditioner_Process(
            &relayCond, (int)relayCmd, DEBOUNCE_TIME_MS, VALIDATION_TIME_MS);

        bool relayOn = (validated != 0);
        bool prevRelay = AppState_GetRelayState();

        if (relayOn != prevRelay) {
            RelayDriver_Set(relayOn);
            AppState_SetRelayState(relayOn);
            printf("[COND] Relay %s\n", relayOn ? "ON" : "OFF");
        }

        // ========== ANALOG ACTUATOR: Servo ==========
        float target = AppState_GetTarget();
        float actual = AppState_GetActual();

        // Signal conditioning: saturate + median + EMA
        float filtered = SignalFilter_Process(&servoFilter, target, ANGLE_MIN, ANGLE_MAX);
        AppState_SetConditioned(filtered);

        // Ramp: smooth movement
        float ramped = SignalFilter_Ramp(actual, filtered, RAMP_STEP);

        // Apply to servo (only if relay is ON = servo has power)
        if (relayOn) {
            ServoDriver_SetAngle(ramped);
        }
        AppState_SetActual(ramped);

        // ========== Alerts ==========
        uint8_t alert = ALERT_NONE;
        if (ramped <= ANGLE_MIN + 0.5f) alert |= ALERT_LIMIT_LO;
        if (ramped >= ANGLE_MAX - 0.5f) alert |= ALERT_LIMIT_HI;
        if (fabsf(ramped - filtered) > 1.0f) alert |= ALERT_RAMPING;
        AppState_SetAlert(alert);

        // LED: on when ramping
        LedDriver_Set((alert & ALERT_RAMPING) != 0);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONDITIONING_TASK_PERIOD_MS));
    }
}
