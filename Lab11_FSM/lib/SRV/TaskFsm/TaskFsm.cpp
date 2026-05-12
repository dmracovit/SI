#include "TaskFsm.h"
#include "FsmCore.h"
#include "JoystickDriver.h"
#include "AppState.h"

// Translate the joystick driver's press classification into FSM events.
static FsmEvent_t buttonEventToFsm(PressEvent_t e)
{
    switch (e) {
        case PRESS_SHORT: return FSM_EV_SHORT_PRESS;
        case PRESS_LONG:  return FSM_EV_LONG_PRESS;
        default:          return FSM_EV_NONE;
    }
}

void TaskFsm_Task(void *pvParameters)
{
    (void)pvParameters;
    TickType_t cycleStart = xTaskGetTickCount();

    for (;;) {
        // Periodic side-effects: blink animation + ARMED-timeout counter.
        FsmCore_Step(FSM_EV_TICK);

        // Discrete joystick events.
        const FsmEvent_t btn = buttonEventToFsm(JoystickDriver_PollEvent());
        if (btn != FSM_EV_NONE) {
            FsmCore_Step(btn);
        }

        vTaskDelayUntil(&cycleStart, pdMS_TO_TICKS(FSM_TICK_PERIOD_MS));
    }
}
