#include <Arduino.h>
#include "SerialIO.h"
#include "LedDriver.h"
#include "ButtonDriver.h"
#include "Signals.h"
#include "TaskButtonMonitor.h"
#include "TaskStatistics.h"
#include "TaskReporter.h"

void setup()
{
    SerialIoInit();
    LedDriver_Init();
    ButtonDriver_Init();
    Signals_Init();

    printf("\n================================\n");
    printf("  Lab 4 - FreeRTOS Scheduler   \n");
    printf("================================\n");

    xTaskCreate(TaskButtonMonitor_Task, "ButtonMonitor", 2048, NULL, 3, NULL);
    xTaskCreate(TaskStatistics_Task,    "Statistics",    2048, NULL, 2, NULL);
    xTaskCreate(TaskReporter_Task,      "Reporter",      4096, NULL, 1, NULL);
}

void loop()
{
    vTaskDelay(portMAX_DELAY);
}
