#include "TaskButtonMonitor.h"
#include "ButtonDriver.h"
#include "LedDriver.h"
#include "Signals.h"

/* 25 ticks × 20ms = 500ms threshold */
#define PRESS_THRESHOLD_TICKS 25
/* 15 ticks × 20ms = 300ms LED on time */
#define LED_OFF_TICKS         15

static bool     wasPressed     = false;
static uint16_t pressTickCount = 0;
static int16_t  ledOffCount    = 0;
static bool     greenActive    = false; // true=green, false=red

void TaskButtonMonitor_Run(void)
{
    bool pressed = ButtonDriver_Read();

    /* Rising edge: button just pressed */
    if (pressed && !wasPressed)
    {
        pressTickCount = 0;
    }

    /* Count ticks while held */
    if (pressed)
    {
        pressTickCount++;
    }

    /* Falling edge: button just released */
    if (!pressed && wasPressed)
    {
        uint16_t duration = (uint16_t)(pressTickCount * 20); // convert ticks → ms
        bool isShort = (pressTickCount < PRESS_THRESHOLD_TICKS);

        /* Publish to Signals for Task2 */
        Signals_SetLastPressDuration(duration);
        Signals_SetIsShortPress(isShort);
        Signals_SetNewPressFlag(true);

        /* Visual feedback */
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

    /* Non-blocking LED off timer */
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
