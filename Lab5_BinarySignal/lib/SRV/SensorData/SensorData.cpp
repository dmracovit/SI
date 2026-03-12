#include "SensorData.h"

SemaphoreHandle_t xSemNewData  = NULL;
SemaphoreHandle_t xSensorMutex = NULL;

// --- Temperature data ---
static float        gTemperature    = 0.0f;
static bool         gTempValid      = false;
static AlertState_t gTempAlert      = ALERT_OFF;
static uint8_t      gTempDebounce   = 0;

// --- Sound data ---
static int          gSoundLevel     = 0;
static AlertState_t gSoundAlert     = ALERT_OFF;
static uint8_t      gSoundDebounce  = 0;

void SensorData_Init(void)
{
    xSemNewData  = xSemaphoreCreateBinary();
    xSensorMutex = xSemaphoreCreateMutex();
}

// ===================== Temperature =====================

void SensorData_SetTempReading(float temp, bool valid)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gTemperature = temp;
    gTempValid   = valid;
    xSemaphoreGive(xSensorMutex);
}

float SensorData_GetTemperature(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    float t = gTemperature;
    xSemaphoreGive(xSensorMutex);
    return t;
}

bool SensorData_IsTempValid(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    bool v = gTempValid;
    xSemaphoreGive(xSensorMutex);
    return v;
}

AlertState_t SensorData_GetTempAlert(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    AlertState_t s = gTempAlert;
    xSemaphoreGive(xSensorMutex);
    return s;
}

void SensorData_SetTempAlert(AlertState_t state)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gTempAlert = state;
    xSemaphoreGive(xSensorMutex);
}

uint8_t SensorData_GetTempDebounce(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    uint8_t c = gTempDebounce;
    xSemaphoreGive(xSensorMutex);
    return c;
}

void SensorData_SetTempDebounce(uint8_t count)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gTempDebounce = count;
    xSemaphoreGive(xSensorMutex);
}

// ===================== Sound =====================

void SensorData_SetSoundReading(int value)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gSoundLevel = value;
    xSemaphoreGive(xSensorMutex);
}

int SensorData_GetSoundLevel(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    int v = gSoundLevel;
    xSemaphoreGive(xSensorMutex);
    return v;
}

AlertState_t SensorData_GetSoundAlert(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    AlertState_t s = gSoundAlert;
    xSemaphoreGive(xSensorMutex);
    return s;
}

void SensorData_SetSoundAlert(AlertState_t state)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gSoundAlert = state;
    xSemaphoreGive(xSensorMutex);
}

uint8_t SensorData_GetSoundDebounce(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    uint8_t c = gSoundDebounce;
    xSemaphoreGive(xSensorMutex);
    return c;
}

void SensorData_SetSoundDebounce(uint8_t count)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gSoundDebounce = count;
    xSemaphoreGive(xSensorMutex);
}
