#pragma once
#include <Arduino.h>

#define JOYSTICK_X_PIN      34
#define JOYSTICK_Y_PIN      35
#define JOYSTICK_SWITCH_PIN 25
#define JOYSTICK_CENTER     1860
#define JOYSTICK_DEADZONE   400

void  JoystickDriver_Init(void);
int   JoystickDriver_ReadX(void);
int   JoystickDriver_ReadY(void);
float JoystickDriver_GetAngle(void);     // 0-180 degrees from joystick direction
bool  JoystickDriver_IsActive(void);     // joystick moved outside deadzone?
bool  JoystickDriver_IsPressed(void);
bool  JoystickDriver_WasPressed(void);
