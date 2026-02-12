#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>

#define LED_PIN 13

void LedInit(void);
void LedOn(void);
void LedOff(void);
void LedToggle(void);
bool LedGetState(void);

#endif
