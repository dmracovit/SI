#pragma once

#define BUTTON_PIN 18

void ButtonDriver_Init(void);
bool ButtonDriver_IsPressed(void);
bool ButtonDriver_WasPressed(void);  // edge detection: true once per press
