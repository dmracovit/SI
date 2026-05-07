#pragma once
#include <Arduino.h>

// Pin 9 uses Timer 2 — does not conflict with Arduino_FreeRTOS scheduler.
#define LED_PWM_PIN  9

void LedPwmDriver_Init(void);
void LedPwmDriver_SetDuty(uint8_t duty);  // 0..255
