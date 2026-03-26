#include "ButtonDriver.h"
#include "GpioDriver.h"

static bool lastState = false;

void ButtonDriver_Init(void)
{
    GpioDriver_PinMode(BUTTON_PIN, INPUT_PULLUP);
    lastState = false;
}

bool ButtonDriver_IsPressed(void)
{
    // Active LOW (pulled up, pressed = LOW)
    return (GpioDriver_Read(BUTTON_PIN) == LOW);
}

bool ButtonDriver_WasPressed(void)
{
    bool current = ButtonDriver_IsPressed();
    bool edge = (current && !lastState);  // rising edge (just pressed)
    lastState = current;
    return edge;
}
