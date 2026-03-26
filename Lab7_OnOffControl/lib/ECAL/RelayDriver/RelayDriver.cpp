#include "RelayDriver.h"
#include "GpioDriver.h"

static bool relayState = false;

void RelayDriver_Init(void)
{
    GpioDriver_PinMode(RELAY_PIN, OUTPUT);
    GpioDriver_Write(RELAY_PIN, LOW);
    relayState = false;
}

void RelayDriver_On(void)
{
    GpioDriver_Write(RELAY_PIN, HIGH);
    relayState = true;
}

void RelayDriver_Off(void)
{
    GpioDriver_Write(RELAY_PIN, LOW);
    relayState = false;
}

void RelayDriver_Set(bool on)
{
    if (on) RelayDriver_On();
    else    RelayDriver_Off();
}

bool RelayDriver_GetState(void)
{
    return relayState;
}
