#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include <Arduino.h>

#define BUTTON_PIN 2

void ButtonDriver_Init(void);
bool ButtonDriver_Read(void);

#endif
