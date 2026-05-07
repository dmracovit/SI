#pragma once
#include <Arduino.h>

#define LDR_PIN  A0

void  LdrSensor_Init(void);
int   LdrSensor_ReadRaw(void);     // raw ADC 0-1023
float LdrSensor_ReadPercent(void); // 0=dark, 100=bright
