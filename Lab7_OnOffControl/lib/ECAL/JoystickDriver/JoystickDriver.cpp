#include "JoystickDriver.h"
#include "GpioDriver.h"

static bool lastSwitch = false;

void JoystickDriver_Init(void)
{
    GpioDriver_PinMode(JOYSTICK_SWITCH_PIN, INPUT_PULLUP);
    lastSwitch = false;
}

bool JoystickDriver_IsPressed(void)
{
    return (GpioDriver_Read(JOYSTICK_SWITCH_PIN) == LOW);
}

bool JoystickDriver_WasPressed(void)
{
    bool current = JoystickDriver_IsPressed();
    bool edge = (current && !lastSwitch);
    lastSwitch = current;
    return edge;
}
