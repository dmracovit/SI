#include "AppState.h"
#include <string.h>

SemaphoreHandle_t xAppMutex = NULL;

static float   gTarget      = 90.0f;
static float   gConditioned = 90.0f;
static float   gActual      = 90.0f;
static uint8_t gAlert       = ALERT_NONE;

static bool  gRelayCmd      = false;
static bool  gRelayState    = false;

static char  gSerialCmd[32] = {0};
static bool  gHasSerialCmd  = false;

void AppState_Init(void)
{
    xAppMutex = xSemaphoreCreateMutex();
}

// ===== Target =====
void AppState_SetTarget(float angle)
{
    if (angle < ANGLE_MIN) angle = ANGLE_MIN;
    if (angle > ANGLE_MAX) angle = ANGLE_MAX;
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gTarget = angle;
    xSemaphoreGive(xAppMutex);
}

float AppState_GetTarget(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    float v = gTarget;
    xSemaphoreGive(xAppMutex);
    return v;
}

// ===== Conditioned =====
void AppState_SetConditioned(float angle)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gConditioned = angle;
    xSemaphoreGive(xAppMutex);
}

float AppState_GetConditioned(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    float v = gConditioned;
    xSemaphoreGive(xAppMutex);
    return v;
}

// ===== Actual =====
void AppState_SetActual(float angle)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gActual = angle;
    xSemaphoreGive(xAppMutex);
}

float AppState_GetActual(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    float v = gActual;
    xSemaphoreGive(xAppMutex);
    return v;
}

// ===== Alert =====
void AppState_SetAlert(uint8_t flags)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gAlert = flags;
    xSemaphoreGive(xAppMutex);
}

uint8_t AppState_GetAlert(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    uint8_t a = gAlert;
    xSemaphoreGive(xAppMutex);
    return a;
}

// ===== Relay =====
void AppState_SetRelayCmd(bool on)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gRelayCmd = on;
    xSemaphoreGive(xAppMutex);
}

bool AppState_GetRelayCmd(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    bool r = gRelayCmd;
    xSemaphoreGive(xAppMutex);
    return r;
}

void AppState_SetRelayState(bool on)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gRelayState = on;
    xSemaphoreGive(xAppMutex);
}

bool AppState_GetRelayState(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    bool r = gRelayState;
    xSemaphoreGive(xAppMutex);
    return r;
}

// ===== Serial =====
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
