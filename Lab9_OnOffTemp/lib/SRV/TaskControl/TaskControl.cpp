#include "TaskControl.h"
#include "AppState.h"
#include "RelayDriver.h"

// ON-OFF control with hysteresis:
//   if temp < (SP - hyst) → relay ON  (heating)
//   if temp > (SP + hyst) → relay OFF
//   else → keep current state (deadband — prevents chatter)

void TaskControl_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        if (AppState_IsSensorValid()) {
            float temp = AppState_GetTemperature();
            float sp   = AppState_GetSetPoint();
            float h    = AppState_GetHysteresis();
            bool current = AppState_GetRelay();

            bool desired = current;
            if (temp < (sp - h))      desired = true;
            else if (temp > (sp + h)) desired = false;

            if (desired != current) {
                RelayDriver_Set(desired);
                AppState_SetRelay(desired);
                printf("[CTRL] Relay %s (temp=%.1f SP=%.1f hyst=%.1f)\n",
                       desired ? "ON" : "OFF", temp, sp, h);
            }
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONTROL_TASK_PERIOD_MS));
    }
}
