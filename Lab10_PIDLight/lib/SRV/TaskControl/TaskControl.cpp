#include "TaskControl.h"
#include "AppState.h"
#include "PidController.h"
#include "LedPwmDriver.h"

static PidState_t pid;

// PID setpoint is in % (0-100). Light sensor reads in %.
// PID output range = 0..255 (PWM duty).
// To convert SP to "PWM space" we scale by 2.55, since 100% light ≈ 255 PWM.

void TaskControl_Task(void *pvParameters)
{
    PidController_Init(&pid, CONTROL_TASK_PERIOD_MS / 1000.0f);

    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        float light = AppState_GetLight();      // 0..100 %
        float sp    = AppState_GetSetPoint();   // 0..100 %
        float kp    = AppState_GetKp();
        float ki    = AppState_GetKi();
        float kd    = AppState_GetKd();

        // Convert setpoint and measurement to PWM scale (0..255)
        float spPwm   = sp    * 2.55f;
        float measPwm = light * 2.55f;

        float out = PidController_Compute(&pid, spPwm, measPwm,
                                          kp, ki, kd, PWM_MIN, PWM_MAX);

        uint8_t duty = (uint8_t)out;
        LedPwmDriver_SetDuty(duty);
        AppState_SetPwmOutput(duty);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONTROL_TASK_PERIOD_MS));
    }
}
