#pragma once
#include <Arduino.h>

#define DHT_PIN 4

void  DhtSensor_Init(void);
bool  DhtSensor_Read(float *temperature, float *humidity);
