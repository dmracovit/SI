#include "TaskStatistics.h"
#include "LedDriver.h"
#include "Signals.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

typedef enum { STATE_IDLE, STATE_BLINKING } StatState_t;

static StatState_t state                = STATE_IDLE;
static int16_t     blinkTogglesRemaining = 0;

void TaskStatistics_Run(void)
{
    if (xSemaphoreTake(xSemRelease, 0) == pdTRUE)
    {
        uint16_t duration = Signals_GetLastPressDuration();
        bool     isShort  = Signals_GetIsShortPress();

        Signals_SetTotalPresses(Signals_GetTotalPresses() + 1);

        if (isShort)
        {
            Signals_SetShortPresses(Signals_GetShortPresses() + 1);
            Signals_SetTotalShortDuration(Signals_GetTotalShortDuration() + duration);
            blinkTogglesRemaining = 10;
        }
        else
        {
            Signals_SetLongPresses(Signals_GetLongPresses() + 1);
            Signals_SetTotalLongDuration(Signals_GetTotalLongDuration() + duration);
            blinkTogglesRemaining = 20;
        }
        state = STATE_BLINKING;
    }

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

void TaskStatistics_Task(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(5));
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        TaskStatistics_Run();
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    }
}
