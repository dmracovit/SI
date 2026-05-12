#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#define FSM_TICK_PERIOD_MS    50
#define DISPLAY_PERIOD_MS     500
#define CMD_PERIOD_MS         50

extern SemaphoreHandle_t xLcdMutex;

void      AppState_Init(void);

void      AppState_BumpTransitions(void);
uint32_t  AppState_GetTransitions(void);

// Serial-command channel
void      AppState_PushCmd(const char *line);
bool      AppState_HasCmd(void);
void      AppState_PopCmd(char *out, size_t cap);
