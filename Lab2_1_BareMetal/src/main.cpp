#include <stdio.h>
#include "SerialIO.h"
#include "LedDriver.h"
#include "ButtonDriver.h"
#include "Signals.h"
#include "Scheduler.h"

void setup()
{
    SerialIoInit();
    LedDriver_Init();
    ButtonDriver_Init();
    Signals_Init();
    Scheduler_Init();

    printf("\n================================\n");
    printf(" Lab 2.1 - Bare Metal Scheduler \n");
    printf("================================\n");

    Scheduler_Start();
}

void loop()
{
}
