#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Arduino.h>

#define LED_GREEN_PIN 10
#define LED_RED_PIN   11

void LedInit(void);
void LedGreenOn(void);
void LedGreenOff(void);
void LedRedOn(void);
void LedRedOff(void);

#endif
