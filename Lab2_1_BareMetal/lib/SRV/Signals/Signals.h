#ifndef SIGNALS_H
#define SIGNALS_H

#include <Arduino.h>

void Signals_Init(void);

void     Signals_SetLastPressDuration(uint16_t duration);
uint16_t Signals_GetLastPressDuration(void);

void Signals_SetNewPressFlag(bool flag);
bool Signals_GetNewPressFlag(void);

void Signals_SetIsShortPress(bool isShort);
bool Signals_GetIsShortPress(void);

void     Signals_SetTotalPresses(uint16_t val);
uint16_t Signals_GetTotalPresses(void);

void     Signals_SetShortPresses(uint16_t val);
uint16_t Signals_GetShortPresses(void);

void     Signals_SetLongPresses(uint16_t val);
uint16_t Signals_GetLongPresses(void);

void     Signals_SetTotalShortDuration(uint32_t val);
uint32_t Signals_GetTotalShortDuration(void);

void     Signals_SetTotalLongDuration(uint32_t val);
uint32_t Signals_GetTotalLongDuration(void);

#endif
