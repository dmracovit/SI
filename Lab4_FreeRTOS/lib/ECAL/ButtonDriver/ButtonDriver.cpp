#include "ButtonDriver.h"
#include "GpioDriver.h"

#define BUTTON_PIN 4

void ButtonDriver_Init(void)
{
    GpioDriver_PinMode(BUTTON_PIN, INPUT_PULLUP);
}

bool ButtonDriver_Read(void)
{
    return (GpioDriver_Read(BUTTON_PIN) == LOW);
}
