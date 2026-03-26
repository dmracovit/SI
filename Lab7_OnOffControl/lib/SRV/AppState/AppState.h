#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// Timing configuration
#define INPUT_TASK_PERIOD_MS        50
#define CONDITIONING_TASK_PERIOD_MS 50
#define DISPLAY_TASK_PERIOD_MS      500

#define DEBOUNCE_TIME_MS            50
#define VALIDATION_TIME_MS          100

typedef enum {
    CMD_OFF = 0,
    CMD_ON  = 1
} CmdState_t;

// Synchronization primitives
extern SemaphoreHandle_t xAppMutex;
extern SemaphoreHandle_t xSemDisplayUpdate;  // signal display to refresh

void       AppState_Init(void);

// Command state: what the user wants (from button/joystick/serial)
void       AppState_SetCommand(CmdState_t cmd);
CmdState_t AppState_GetCommand(void);

// Conditioned state: after debounce + validation
void       AppState_SetConditioned(CmdState_t cmd);
CmdState_t AppState_GetConditioned(void);

// Actual relay state
void       AppState_SetRelayState(bool on);
bool       AppState_GetRelayState(void);

// Toggle count (how many times relay switched)
void       AppState_IncrementToggleCount(void);
uint32_t   AppState_GetToggleCount(void);

// Serial command buffer
void       AppState_SetSerialCmd(const char *cmd);
bool       AppState_HasSerialCmd(void);
void       AppState_GetSerialCmd(char *buf, size_t len);
