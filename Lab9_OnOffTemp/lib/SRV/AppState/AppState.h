#pragma once
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// Timing
#define SENSOR_TASK_PERIOD_MS    2000
#define CONTROL_TASK_PERIOD_MS   500
#define DISPLAY_TASK_PERIOD_MS   1000
#define CMD_TASK_PERIOD_MS       50

// Default control parameters
// Relay turns OFF when temp > SP + hyst (= 27 C with these defaults)
// Relay turns ON  when temp < SP - hyst (= 25 C with these defaults)
#define DEFAULT_SETPOINT     26.0f
#define DEFAULT_HYSTERESIS   1.0f
#define SETPOINT_MIN         0.0f
#define SETPOINT_MAX         80.0f
#define HYST_MIN             0.0f
#define HYST_MAX             20.0f

extern SemaphoreHandle_t xAppMutex;

void AppState_Init(void);

// Sensor data
void  AppState_SetSensor(float temp, float hum, bool valid);
float AppState_GetTemperature(void);
float AppState_GetHumidity(void);
bool  AppState_IsSensorValid(void);

// Control parameters
void  AppState_SetSetPoint(float sp);
float AppState_GetSetPoint(void);
void  AppState_SetHysteresis(float h);
float AppState_GetHysteresis(void);

// Relay state
void AppState_SetRelay(bool on);
bool AppState_GetRelay(void);

// Serial command
void AppState_SetSerialCmd(const char *cmd);
bool AppState_HasSerialCmd(void);
void AppState_GetSerialCmd(char *buf, size_t len);
