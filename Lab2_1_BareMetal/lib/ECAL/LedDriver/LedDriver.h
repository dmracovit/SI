#ifndef LED_DRIVER_H
#define LED_DRIVER_H

/* ECAL - LedDriver
 * Controls three LEDs connected via current-limiting resistors (220Ω).
 *   Green  D10 — short press indicator
 *   Red    D11 — long press indicator
 *   Yellow D12 — blink counter (Task2)
 */

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
