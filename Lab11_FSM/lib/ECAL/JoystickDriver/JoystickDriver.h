#pragma once
#include <Arduino.h>

#define JOYSTICK_SWITCH_PIN  17
#define LONG_PRESS_MS        800   // press held >= this counts as "long"

typedef enum {
    PRESS_NONE  = 0,
    PRESS_SHORT = 1,    // released after a brief tap
    PRESS_LONG  = 2     // released after holding >= LONG_PRESS_MS
} PressEvent_t;

void          JoystickDriver_Init(void);
bool          JoystickDriver_IsPressed(void);

// Returns a press event when the button is RELEASED. Returns PRESS_NONE
// while the user is still holding it down or while idle.
PressEvent_t  JoystickDriver_PollEvent(void);
