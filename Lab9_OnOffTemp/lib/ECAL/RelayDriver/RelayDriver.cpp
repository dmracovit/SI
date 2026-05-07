#include "RelayDriver.h"
#include "GpioDriver.h"

static bool state = false;

void RelayDriver_Init(void)
{
    GpioDriver_PinMode(RELAY_PIN, OUTPUT);
    // active-LOW: HIGH on pin = relay OFF
    GpioDriver_Write(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW);
    state = false;
}

void RelayDriver_Set(bool on)
{
    state = on;
    uint8_t level;
    if (RELAY_ACTIVE_LOW) level = on ? LOW  : HIGH;
    else                  level = on ? HIGH : LOW;
    GpioDriver_Write(RELAY_PIN, level);
}

bool RelayDriver_GetState(void) { return state; }
