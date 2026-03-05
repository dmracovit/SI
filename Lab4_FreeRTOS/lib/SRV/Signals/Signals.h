#pragma once
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

extern SemaphoreHandle_t xSemPress;
extern SemaphoreHandle_t xSemRelease;
extern SemaphoreHandle_t xSignalsMutex;

void     Signals_Init(void);

uint16_t Signals_GetLastPressDuration(void);
void     Signals_SetLastPressDuration(uint16_t val);

bool     Signals_GetIsShortPress(void);
void     Signals_SetIsShortPress(bool val);

uint32_t Signals_GetTotalPresses(void);
void     Signals_SetTotalPresses(uint32_t val);

uint32_t Signals_GetShortPresses(void);
void     Signals_SetShortPresses(uint32_t val);

uint32_t Signals_GetLongPresses(void);
void     Signals_SetLongPresses(uint32_t val);

uint32_t Signals_GetTotalShortDuration(void);
void     Signals_SetTotalShortDuration(uint32_t val);

uint32_t Signals_GetTotalLongDuration(void);
void     Signals_SetTotalLongDuration(uint32_t val);

void     Signals_ResetAll(void);
