#include "TaskControl.h"
#include "AppState.h"
#include "PidController.h"
#include "LedPwmDriver.h"

// Setpoint and measurement are tracked in percent (0..100). The PID is
// evaluated directly in PWM units (0..255) so the saturation limits match
// the actuator's natural domain — this avoids re-scaling inside the PID.
#define LIGHT_PCT_TO_PWM   2.55f   // 100 % maps to 255 duty

static PidController_t pidCtx;

void TaskControl_Task(void *pvParameters)
{
    (void)pvParameters;

    const float dtSec = (float)CONTROL_TASK_PERIOD_MS / 1000.0f;
    PidController_Init(&pidCtx, dtSec);

    TickType_t cycleStart = xTaskGetTickCount();

    for (;;)
    {
        // ---- Snapshot inputs (mutex-protected) ----
        const float lightPct = AppState_GetLight();
        const float spPct    = AppState_GetSetPoint();
        const float kp       = AppState_GetKp();
        const float ki       = AppState_GetKi();
        const float kd       = AppState_GetKd();

        // ---- Move into PWM domain (0..255) ----
        const float spDuty   = spPct    * LIGHT_PCT_TO_PWM;
        const float measDuty = lightPct * LIGHT_PCT_TO_PWM;

        // ---- Run controller ----
        const float duty = PidController_Compute(
            &pidCtx, spDuty, measDuty, kp, ki, kd, PWM_MIN, PWM_MAX);

        const uint8_t pwm = (uint8_t)duty;
        LedPwmDriver_SetDuty(pwm);
        AppState_SetPwmOutput(pwm);

        vTaskDelayUntil(&cycleStart, pdMS_TO_TICKS(CONTROL_TASK_PERIOD_MS));
    }
}
