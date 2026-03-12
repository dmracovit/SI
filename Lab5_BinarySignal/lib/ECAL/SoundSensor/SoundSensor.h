#pragma once

#define SOUND_SENSOR_PIN 34  // ADC1_CH6 (GPIO34, input-only)

void SoundSensor_Init(void);
int  SoundSensor_Read(void);
