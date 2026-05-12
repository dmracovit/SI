#include "AppState.h"
#include <string.h>

SemaphoreHandle_t xLcdMutex = NULL;
static SemaphoreHandle_t xDataMutex = NULL;

static uint32_t transitionCount = 0;
static char     cmdBuf[32]      = {0};
static bool     cmdReady        = false;

void AppState_Init(void)
{
    xLcdMutex  = xSemaphoreCreateMutex();
    xDataMutex = xSemaphoreCreateMutex();
}

void AppState_BumpTransitions(void)
{
    xSemaphoreTake(xDataMutex, portMAX_DELAY);
    transitionCount++;
    xSemaphoreGive(xDataMutex);
}

uint32_t AppState_GetTransitions(void)
{
    xSemaphoreTake(xDataMutex, portMAX_DELAY);
    uint32_t n = transitionCount;
    xSemaphoreGive(xDataMutex);
    return n;
}

void AppState_PushCmd(const char *line)
{
    xSemaphoreTake(xDataMutex, portMAX_DELAY);
    strncpy(cmdBuf, line, sizeof(cmdBuf) - 1);
    cmdBuf[sizeof(cmdBuf) - 1] = '\0';
    cmdReady = true;
    xSemaphoreGive(xDataMutex);
}

bool AppState_HasCmd(void)
{
    xSemaphoreTake(xDataMutex, portMAX_DELAY);
    bool r = cmdReady;
    xSemaphoreGive(xDataMutex);
    return r;
}

void AppState_PopCmd(char *out, size_t cap)
{
    xSemaphoreTake(xDataMutex, portMAX_DELAY);
    strncpy(out, cmdBuf, cap - 1);
    out[cap - 1] = '\0';
    cmdReady = false;
    xSemaphoreGive(xDataMutex);
}
