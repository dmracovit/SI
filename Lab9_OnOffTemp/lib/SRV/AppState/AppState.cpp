#include "AppState.h"
#include <string.h>

SemaphoreHandle_t xAppMutex = NULL;

static float gTemperature = 0.0f;
static float gHumidity    = 0.0f;
static bool  gSensorValid = false;

static float gSetPoint    = DEFAULT_SETPOINT;
static float gHysteresis  = DEFAULT_HYSTERESIS;

static bool  gRelayOn     = false;

static char  gSerialCmd[32] = {0};
static bool  gHasSerialCmd  = false;

void AppState_Init(void) { xAppMutex = xSemaphoreCreateMutex(); }

void AppState_SetSensor(float temp, float hum, bool valid)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gTemperature = temp; gHumidity = hum; gSensorValid = valid;
    xSemaphoreGive(xAppMutex);
}

float AppState_GetTemperature(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    float v = gTemperature;
    xSemaphoreGive(xAppMutex);
    return v;
}

float AppState_GetHumidity(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    float v = gHumidity;
    xSemaphoreGive(xAppMutex);
    return v;
}

bool AppState_IsSensorValid(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    bool v = gSensorValid;
    xSemaphoreGive(xAppMutex);
    return v;
}

void AppState_SetSetPoint(float sp)
{
    if (sp < SETPOINT_MIN) sp = SETPOINT_MIN;
    if (sp > SETPOINT_MAX) sp = SETPOINT_MAX;
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gSetPoint = sp;
    xSemaphoreGive(xAppMutex);
}

float AppState_GetSetPoint(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    float v = gSetPoint;
    xSemaphoreGive(xAppMutex);
    return v;
}

void AppState_SetHysteresis(float h)
{
    if (h < HYST_MIN) h = HYST_MIN;
    if (h > HYST_MAX) h = HYST_MAX;
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gHysteresis = h;
    xSemaphoreGive(xAppMutex);
}

float AppState_GetHysteresis(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    float v = gHysteresis;
    xSemaphoreGive(xAppMutex);
    return v;
}

void AppState_SetRelay(bool on)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    gRelayOn = on;
    xSemaphoreGive(xAppMutex);
}

bool AppState_GetRelay(void)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    bool v = gRelayOn;
    xSemaphoreGive(xAppMutex);
    return v;
}

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
    bool v = gHasSerialCmd;
    xSemaphoreGive(xAppMutex);
    return v;
}

void AppState_GetSerialCmd(char *buf, size_t len)
{
    xSemaphoreTake(xAppMutex, portMAX_DELAY);
    strncpy(buf, gSerialCmd, len - 1);
    buf[len - 1] = '\0';
    gHasSerialCmd = false;
    xSemaphoreGive(xAppMutex);
}
