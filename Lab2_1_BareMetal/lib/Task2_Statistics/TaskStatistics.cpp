#include "TaskStatistics.h"
#include "LedDriver.h"
#include "Signals.h"

typedef enum {
    STATE_IDLE,
    STATE_BLINKING
} StatState_t;

static StatState_t state = STATE_IDLE;
static int16_t blinkTogglesRemaining = 0;

void TaskStatistics_Run(void)
{
    // Check for new press in ANY state (don't miss presses during blinking)
    if (Signals_GetNewPressFlag())
    {
        uint16_t duration = Signals_GetLastPressDuration();
        bool isShort = Signals_GetIsShortPress();

        Signals_SetTotalPresses(Signals_GetTotalPresses() + 1);

        if (isShort)
        {
            Signals_SetShortPresses(Signals_GetShortPresses() + 1);
            Signals_SetTotalShortDuration(Signals_GetTotalShortDuration() + duration);
            blinkTogglesRemaining = 10; // 5 blinks = 10 toggles
        }
        else
        {
            Signals_SetLongPresses(Signals_GetLongPresses() + 1);
            Signals_SetTotalLongDuration(Signals_GetTotalLongDuration() + duration);
            blinkTogglesRemaining = 20; // 10 blinks = 20 toggles
        }

        Signals_SetNewPressFlag(false);
        state = STATE_BLINKING;
    }

    // Handle blinking
    if (state == STATE_BLINKING)
    {
        LedDriver_YellowToggle();
        blinkTogglesRemaining--;

        if (blinkTogglesRemaining <= 0)
        {
            LedDriver_YellowOff();
            state = STATE_IDLE;
        }
    }
}
