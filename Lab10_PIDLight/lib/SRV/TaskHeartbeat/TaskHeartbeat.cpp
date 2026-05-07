#include "TaskHeartbeat.h"
#include "GpioDriver.h"

void TaskHeartbeat_Task(void *pvParameters)
{
    (void)pvParameters;
    GpioDriver_PinMode(HEARTBEAT_PIN, OUTPUT);
    TickType_t xLast = xTaskGetTickCount();
    for (;;) {
        GpioDriver_Toggle(HEARTBEAT_PIN);
        vTaskDelayUntil(&xLast, pdMS_TO_TICKS(HEARTBEAT_PERIOD_MS));
    }
}
