#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// Timing
#define INPUT_TASK_PERIOD_MS        50
#define CONDITIONING_TASK_PERIOD_MS 50
#define DISPLAY_TASK_PERIOD_MS      500

// Servo limits
#define ANGLE_MIN   0.0f
#define ANGLE_MAX   180.0f

// Ramp: max degrees per conditioning cycle (50ms)
// 2.0 deg/cycle = 40 deg/sec → full sweep 0-180 in ~4.5s
#define RAMP_STEP   2.0f

// Debounce for relay (time-based, from Lab7)
#define DEBOUNCE_TIME_MS    50
#define VALIDATION_TIME_MS  100

extern SemaphoreHandle_t xAppMutex;

void  AppState_Init(void);

// Target angle (what user commands)
void  AppState_SetTarget(float angle);
float AppState_GetTarget(void);

// Conditioned angle (after filter + ramp)
void  AppState_SetConditioned(float angle);
float AppState_GetConditioned(void);

// Actual servo angle (what servo is at)
void  AppState_SetActual(float angle);
float AppState_GetActual(void);

// Alert flags
typedef enum {
    ALERT_NONE     = 0,
    ALERT_LIMIT_LO = 1,
    ALERT_LIMIT_HI = 2,
    ALERT_RAMPING  = 4
} AlertFlag_t;

void     AppState_SetAlert(uint8_t flags);
uint8_t  AppState_GetAlert(void);

// Relay (binary actuator)
void  AppState_SetRelayCmd(bool on);
bool  AppState_GetRelayCmd(void);
void  AppState_SetRelayState(bool on);
bool  AppState_GetRelayState(void);

// Serial command buffer
void AppState_SetSerialCmd(const char *cmd);
bool AppState_HasSerialCmd(void);
void AppState_GetSerialCmd(char *buf, size_t len);
