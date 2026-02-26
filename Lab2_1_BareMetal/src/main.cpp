#include <stdio.h>
#include "SerialIO.h"
#include "LedDriver.h"
#include "ButtonDriver.h"
#include "Signals.h"
#include "Scheduler.h"

void setup()
{
    /* Init hardware drivers */
    SerialIoInit();
    LedDriver_Init();
    ButtonDriver_Init();

    /* Init application services */
    Signals_Init();
    Scheduler_Init();

    /* Welcome banner */
    printf("\n================================\n");
    printf(" Lab 2.1 - Bare Metal Scheduler \n");
    printf("================================\n");

    /* Start Timer1 ISR — scheduler begins running */
    Scheduler_Start();
}

void loop()
{
    /* All work is done inside the Timer1 ISR via Scheduler_Loop(). */
}
