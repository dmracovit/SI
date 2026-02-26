#ifndef SIGNALS_H
#define SIGNALS_H

/* SRV - Signals
 * Shared data bus between tasks, implemented as global variables
 * with getter/setter functions.
 *
 * Data flow:
 *   Task1 (writes) → gLastPressDuration, gNewPressFlag, gIsShortPress
 *   Task2 (reads/writes) → gTotalPresses, gShortPresses, gLongPresses,
 *                           gTotalShortDuration, gTotalLongDuration
 *   Task3 (reads + resets) → all statistics counters
 *
 * No mutexes needed: bare-metal sequential scheduler,
 * tasks never run concurrently.
 */

#include <Arduino.h>

void Signals_Init(void);

/* Task1 → Task2 */
void     Signals_SetLastPressDuration(uint16_t duration);
uint16_t Signals_GetLastPressDuration(void);

void Signals_SetNewPressFlag(bool flag);
bool Signals_GetNewPressFlag(void);

void Signals_SetIsShortPress(bool isShort);
bool Signals_GetIsShortPress(void);

/* Task2 → Task3 */
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
