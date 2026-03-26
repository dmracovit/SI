#include "TaskConditioning.h"
#include "AppState.h"
#include "SignalConditioner.h"
#include "RelayDriver.h"
#include "LedDriver.h"

static CondState_t condState;

void TaskConditioning_Task(void *pvParameters)
{
    SignalConditioner_Init(&condState);

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        // Read current command
        CmdState_t rawCmd = AppState_GetCommand();

        // Apply signal conditioning (debounce + validation)
        int result = SignalConditioner_Process(
            &condState, (int)rawCmd, DEBOUNCE_TIME_MS, VALIDATION_TIME_MS);

        CmdState_t conditioned = (CmdState_t)result;
        CmdState_t previous    = AppState_GetConditioned();

        // Update conditioned state
        AppState_SetConditioned(conditioned);

        // If conditioned state changed → actuate relay
        if (conditioned != previous) {
            bool relayOn = (conditioned == CMD_ON);
            RelayDriver_Set(relayOn);
            LedDriver_Set(relayOn);
            AppState_SetRelayState(relayOn);
            AppState_IncrementToggleCount();

            printf("[COND] Relay %s (toggle #%lu)\n",
                   relayOn ? "ON" : "OFF",
                   (unsigned long)AppState_GetToggleCount());
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONDITIONING_TASK_PERIOD_MS));
    }
}
