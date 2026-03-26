#pragma once

#define JOYSTICK_SWITCH_PIN 25

void JoystickDriver_Init(void);
bool JoystickDriver_IsPressed(void);
bool JoystickDriver_WasPressed(void);  // edge detection: true once per press
