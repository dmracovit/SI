#include "Signals.h"

static uint16_t gLastPressDuration;
static bool     gNewPressFlag;
static bool     gIsShortPress;

static uint16_t gTotalPresses;
static uint16_t gShortPresses;
static uint16_t gLongPresses;
static uint32_t gTotalShortDuration;
static uint32_t gTotalLongDuration;

void Signals_Init(void)
{
    gLastPressDuration = 0;
    gNewPressFlag = false;
    gIsShortPress = false;
    gTotalPresses = 0;
    gShortPresses = 0;
    gLongPresses = 0;
    gTotalShortDuration = 0;
    gTotalLongDuration = 0;
}

// Task1 -> Task2
void     Signals_SetLastPressDuration(uint16_t d) { gLastPressDuration = d; }
uint16_t Signals_GetLastPressDuration(void)       { return gLastPressDuration; }

void Signals_SetNewPressFlag(bool f) { gNewPressFlag = f; }
bool Signals_GetNewPressFlag(void)   { return gNewPressFlag; }

void Signals_SetIsShortPress(bool s) { gIsShortPress = s; }
bool Signals_GetIsShortPress(void)   { return gIsShortPress; }

// Task2 -> Task3
void     Signals_SetTotalPresses(uint16_t v) { gTotalPresses = v; }
uint16_t Signals_GetTotalPresses(void)       { return gTotalPresses; }

void     Signals_SetShortPresses(uint16_t v) { gShortPresses = v; }
uint16_t Signals_GetShortPresses(void)       { return gShortPresses; }

void     Signals_SetLongPresses(uint16_t v) { gLongPresses = v; }
uint16_t Signals_GetLongPresses(void)       { return gLongPresses; }

void     Signals_SetTotalShortDuration(uint32_t v) { gTotalShortDuration = v; }
uint32_t Signals_GetTotalShortDuration(void)       { return gTotalShortDuration; }

void     Signals_SetTotalLongDuration(uint32_t v) { gTotalLongDuration = v; }
uint32_t Signals_GetTotalLongDuration(void)       { return gTotalLongDuration; }
