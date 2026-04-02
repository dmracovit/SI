#pragma once
#include <Arduino.h>

#define SERVO_PIN       19
#define SERVO_MIN_US    500   // pulse width at 0 degrees
#define SERVO_MAX_US    2500  // pulse width at 180 degrees
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
#define SERVO_PWM_FREQ  50    // 50 Hz standard servo
#define SERVO_PWM_CH    0     // LEDC channel
#define SERVO_PWM_RES   16    // 16-bit resolution

void  ServoDriver_Init(void);
void  ServoDriver_SetAngle(float angle);  // 0.0 - 180.0
float ServoDriver_GetAngle(void);
