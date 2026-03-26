#include "AppState.h"
#include <string.h>

SemaphoreHandle_t xAppMutex        = NULL;
SemaphoreHandle_t xSemDisplayUpdate = NULL;

static CmdState_t gCommandState     = CMD_OFF;
static CmdState_t gConditionedState = CMD_OFF;
static bool        gRelayOn         = false;
static uint32_t    gToggleCount     = 0;

static char  gSerialCmd[32] = {0};
static bool  gHasSerialCmd  = false;

void AppState_Init(void)
{
    xAppMutex        = xSemaphoreCreateMutex();
    xSemDisplayUpdate = xSemaphoreCreateBinary();
}

// ===== Command =====
void AppState_SetCommand(CmdState_t cmd)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gCommandState = cmd;
    xSemaphoreGive(xAppMutex);
}

CmdState_t AppState_GetCommand(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    CmdState_t c = gCommandState;
    xSemaphoreGive(xAppMutex);
    return c;
}

// ===== Conditioned =====
void AppState_SetConditioned(CmdState_t cmd)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gConditionedState = cmd;
    xSemaphoreGive(xAppMutex);
}

CmdState_t AppState_GetConditioned(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    CmdState_t c = gConditionedState;
    xSemaphoreGive(xAppMutex);
    return c;
}

// ===== Relay =====
void AppState_SetRelayState(bool on)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gRelayOn = on;
    xSemaphoreGive(xAppMutex);
}

bool AppState_GetRelayState(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    bool r = gRelayOn;
    xSemaphoreGive(xAppMutex);
    return r;
}

// ===== Toggle count =====
void AppState_IncrementToggleCount(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gToggleCount++;
    xSemaphoreGive(xAppMutex);
}

uint32_t AppState_GetToggleCount(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    uint32_t c = gToggleCount;
    xSemaphoreGive(xAppMutex);
    return c;
}

// ===== Serial command =====
void AppState_SetSerialCmd(const char *cmd)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    strncpy(gSerialCmd, cmd, sizeof(gSerialCmd) - 1);
    gSerialCmd[sizeof(gSerialCmd) - 1] = '\0';
    gHasSerialCmd = true;
    xSemaphoreGive(xAppMutex);
}

bool AppState_HasSerialCmd(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    bool h = gHasSerialCmd;
    xSemaphoreGive(xAppMutex);
    return h;
}

void AppState_GetSerialCmd(char *buf, size_t len)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    strncpy(buf, gSerialCmd, len - 1);
    buf[len - 1] = '\0';
    gHasSerialCmd = false;
    xSemaphoreGive(xAppMutex);
}
