#include "JoystickDriver.h"
#include "GpioDriver.h"
#include <math.h>

static bool lastSwitch = false;

void JoystickDriver_Init(void)
{
    GpioDriver_PinMode(JOYSTICK_X_PIN, INPUT);
    GpioDriver_PinMode(JOYSTICK_Y_PIN, INPUT);
    GpioDriver_PinMode(JOYSTICK_SWITCH_PIN, INPUT_PULLUP);
    lastSwitch = false;
}

int JoystickDriver_ReadX(void)
{
    return GpioDriver_AnalogRead(JOYSTICK_X_PIN);
}

int JoystickDriver_ReadY(void)
{
    return GpioDriver_AnalogRead(JOYSTICK_Y_PIN);
}

bool JoystickDriver_IsActive(void)
{
    int x = JoystickDriver_ReadX() - JOYSTICK_CENTER;
    int y = JoystickDriver_ReadY() - JOYSTICK_CENTER;
    float dist = sqrtf((float)(x * x + y * y));
    return (dist > JOYSTICK_DEADZONE);
}

float JoystickDriver_GetAngle(void)
{
    int x = JoystickDriver_ReadX() - JOYSTICK_CENTER;
    int y = JoystickDriver_ReadY() - JOYSTICK_CENTER;

    // atan2 gives -PI..PI, convert to 0..360
    float rad = atan2f((float)y, (float)x);
    float deg = rad * 180.0f / M_PI;
    if (deg < 0) deg += 360.0f;

    // Map 0-360 joystick angle to 0-180 servo angle
    float servo = deg / 2.0f;
    return servo;
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
