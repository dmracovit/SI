#include <Arduino.h>
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

    printf("Lab 3.2 - Bare Metal Scheduler\n");
    printf("Button D2, LEDs: green D10, red D11, yellow D12\n");

    Scheduler_Start();
}

void loop()
{
    // All work done by scheduler in Timer1 ISR
}
