#pragma once

#define LED_PIN 4

void LedDriver_Init(void);
void LedDriver_On(void);
void LedDriver_Off(void);
void LedDriver_Set(bool on);
