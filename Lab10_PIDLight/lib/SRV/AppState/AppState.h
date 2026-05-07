#pragma once
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Timing
#define SENSOR_TASK_PERIOD_MS  200
#define CONTROL_TASK_PERIOD_MS 200
#define DISPLAY_TASK_PERIOD_MS 500
#define CMD_TASK_PERIOD_MS     50

// Default PID parameters
// Optical loop is nearly delay-free → very gain-sensitive.
// These are conservative values, tune via serial.
#define DEFAULT_SETPOINT 50.0f   // % light
#define DEFAULT_KP       0.5f
#define DEFAULT_KI       0.1f
#define DEFAULT_KD       0.02f

// EMA filter alpha for sensor smoothing
#define LDR_EMA_ALPHA    0.35f

// PWM saturation
#define PWM_MIN  0
#define PWM_MAX  255

extern SemaphoreHandle_t xAppMutex;

void AppState_Init(void);

// Sensor (light, 0-100%)
void  AppState_SetLight(float percent);
float AppState_GetLight(void);

// PID setpoint and gains
void  AppState_SetSetPoint(float sp);
float AppState_GetSetPoint(void);
void  AppState_SetKp(float v);
void  AppState_SetKi(float v);
void  AppState_SetKd(float v);
float AppState_GetKp(void);
float AppState_GetKi(void);
float AppState_GetKd(void);

// PID output (0-255)
void     AppState_SetPwmOutput(uint8_t duty);
uint8_t  AppState_GetPwmOutput(void);

// Serial command
void AppState_SetSerialCmd(const char *cmd);
bool AppState_HasSerialCmd(void);
void AppState_GetSerialCmd(char *buf, size_t len);
