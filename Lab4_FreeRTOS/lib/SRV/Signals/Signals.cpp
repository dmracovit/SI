#include "Signals.h"

SemaphoreHandle_t xSemPress    = NULL;
SemaphoreHandle_t xSemRelease  = NULL;
SemaphoreHandle_t xSignalsMutex = NULL;

static uint16_t gLastPressDuration  = 0;
static bool     gIsShortPress       = false;
static uint32_t gTotalPresses       = 0;
static uint32_t gShortPresses       = 0;
static uint32_t gLongPresses        = 0;
static uint32_t gTotalShortDuration = 0;
static uint32_t gTotalLongDuration  = 0;

void Signals_Init(void)
{
    xSemPress    = xSemaphoreCreateBinary();
    xSemRelease  = xSemaphoreCreateBinary();
    xSignalsMutex = xSemaphoreCreateMutex();
}

uint16_t Signals_GetLastPressDuration(void)
{
    uint16_t val;
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    val = gLastPressDuration;
    xSemaphoreGive(xSignalsMutex);
    return val;
}

void Signals_SetLastPressDuration(uint16_t val)
{
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    gLastPressDuration = val;
    xSemaphoreGive(xSignalsMutex);
}

bool Signals_GetIsShortPress(void)
{
    bool val;
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    val = gIsShortPress;
    xSemaphoreGive(xSignalsMutex);
    return val;
}

void Signals_SetIsShortPress(bool val)
{
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    gIsShortPress = val;
    xSemaphoreGive(xSignalsMutex);
}

uint32_t Signals_GetTotalPresses(void)
{
    uint32_t val;
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    val = gTotalPresses;
    xSemaphoreGive(xSignalsMutex);
    return val;
}

void Signals_SetTotalPresses(uint32_t val)
{
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    gTotalPresses = val;
    xSemaphoreGive(xSignalsMutex);
}

uint32_t Signals_GetShortPresses(void)
{
    uint32_t val;
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    val = gShortPresses;
    xSemaphoreGive(xSignalsMutex);
    return val;
}

void Signals_SetShortPresses(uint32_t val)
{
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    gShortPresses = val;
    xSemaphoreGive(xSignalsMutex);
}

uint32_t Signals_GetLongPresses(void)
{
    uint32_t val;
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    val = gLongPresses;
    xSemaphoreGive(xSignalsMutex);
    return val;
}

void Signals_SetLongPresses(uint32_t val)
{
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    gLongPresses = val;
    xSemaphoreGive(xSignalsMutex);
}

uint32_t Signals_GetTotalShortDuration(void)
{
    uint32_t val;
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    val = gTotalShortDuration;
    xSemaphoreGive(xSignalsMutex);
    return val;
}

void Signals_SetTotalShortDuration(uint32_t val)
{
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    gTotalShortDuration = val;
    xSemaphoreGive(xSignalsMutex);
}

uint32_t Signals_GetTotalLongDuration(void)
{
    uint32_t val;
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    val = gTotalLongDuration;
    xSemaphoreGive(xSignalsMutex);
    return val;
}

void Signals_SetTotalLongDuration(uint32_t val)
{
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    gTotalLongDuration = val;
    xSemaphoreGive(xSignalsMutex);
}

void Signals_ResetAll(void)
{
    xSemaphoreTake(xSignalsMutex, portMAX_DELAY);
    gTotalPresses       = 0;
    gShortPresses       = 0;
    gLongPresses        = 0;
    gTotalShortDuration = 0;
    gTotalLongDuration  = 0;
    xSemaphoreGive(xSignalsMutex);
}
