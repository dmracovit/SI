#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <Arduino.h>

#define LED_GREEN_PIN  10
#define LED_RED_PIN    11
#define LED_YELLOW_PIN 12

void LedDriver_Init(void);
void LedDriver_GreenOn(void);
void LedDriver_GreenOff(void);
void LedDriver_RedOn(void);
void LedDriver_RedOff(void);
void LedDriver_YellowOn(void);
void LedDriver_YellowOff(void);
void LedDriver_YellowToggle(void);

#endif
