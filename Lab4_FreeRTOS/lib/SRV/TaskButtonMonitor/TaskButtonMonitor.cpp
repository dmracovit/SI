#include "TaskButtonMonitor.h"
#include "ButtonDriver.h"
#include "LedDriver.h"
#include "Signals.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PRESS_THRESHOLD_TICKS 25
#define LED_OFF_TICKS         15

static bool     wasPressed     = false;
static uint16_t pressTickCount = 0;
static int16_t  ledOffCount    = 0;
static bool     greenActive    = false;

void TaskButtonMonitor_Run(void)
{
    bool pressed = ButtonDriver_Read();

    if (pressed && !wasPressed)
    {
        pressTickCount = 0;
        xSemaphoreGive(xSemPress);
        printf("[MONITOR] Button pressed\n");
    }

    if (pressed)
        pressTickCount++;

    if (!pressed && wasPressed)
    {
        uint16_t duration = (uint16_t)(pressTickCount * 20);
        bool isShort = (pressTickCount < PRESS_THRESHOLD_TICKS);

        Signals_SetLastPressDuration(duration);
        Signals_SetIsShortPress(isShort);
        xSemaphoreGive(xSemRelease);

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

        printf("[MONITOR] Button released! Duration: %u ms (%s)\n",
               duration, isShort ? "SHORT" : "LONG");
    }

    wasPressed = pressed;

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

void TaskButtonMonitor_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        TaskButtonMonitor_Run();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(20));
    }
}
