#include "JoystickDriver.h"
#include "GpioDriver.h"

static bool          wasHeld    = false;
static unsigned long pressStart = 0;

void JoystickDriver_Init(void)
{
    GpioDriver_PinMode(JOYSTICK_SWITCH_PIN, INPUT_PULLUP);
    wasHeld    = false;
    pressStart = 0;
}

bool JoystickDriver_IsPressed(void)
{
    return (GpioDriver_Read(JOYSTICK_SWITCH_PIN) == LOW);
}

// Event detector with built-in long-press discrimination:
//   - record millis() on the falling edge (start of press)
//   - on the rising edge, compare hold time against LONG_PRESS_MS and
//     return PRESS_SHORT or PRESS_LONG accordingly.
PressEvent_t JoystickDriver_PollEvent(void)
{
    const bool nowHeld = JoystickDriver_IsPressed();
    const unsigned long now = millis();

    PressEvent_t result = PRESS_NONE;

    if (nowHeld && !wasHeld) {
        pressStart = now;
    } else if (!nowHeld && wasHeld) {
        const unsigned long held = now - pressStart;
        result = (held >= LONG_PRESS_MS) ? PRESS_LONG : PRESS_SHORT;
    }
    wasHeld = nowHeld;
    return result;
}
