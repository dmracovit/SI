#include "TaskButtonMonitor.h"
#include "ButtonDriver.h"
#include "LedDriver.h"
#include "Signals.h"

#define PRESS_THRESHOLD_TICKS 25  // 25 * 20ms = 500ms
#define LED_OFF_TICKS         15  // 15 * 20ms = 300ms

static bool wasPressed = false;
static uint16_t pressTickCount = 0;
static int16_t ledOffCount = 0;
static bool greenActive = false;

void TaskButtonMonitor_Run(void)
{
    bool pressed = ButtonDriver_Read();

    // Rising edge: button just pressed
    if (pressed && !wasPressed)
    {
        pressTickCount = 0;
    }

    // Count while held
    if (pressed)
    {
        pressTickCount++;
    }

    // Falling edge: button just released
    if (!pressed && wasPressed)
    {
        uint16_t duration = pressTickCount * 20; // ms
        bool isShort = (pressTickCount < PRESS_THRESHOLD_TICKS);

        Signals_SetLastPressDuration(duration);
        Signals_SetIsShortPress(isShort);
        Signals_SetNewPressFlag(true);

        if (isShort)
        {
            LedDriver_GreenOn();
            greenActive = true;
        }
        else
        {
            LedDriver_RedOn();
            greenActive = false;
        }
        ledOffCount = LED_OFF_TICKS;
    }

    wasPressed = pressed;

    // LED off timer
    if (ledOffCount > 0)
    {
        ledOffCount--;
        if (ledOffCount == 0)
        {
            if (greenActive)
                LedDriver_GreenOff();
            else
                LedDriver_RedOff();
        }
    }
}
