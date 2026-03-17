#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// --- Temperature thresholds (DS18B20, digital) ---
#define TEMP_THRESH_HIGH    26.0f
#define TEMP_THRESH_LOW     24.0f

// --- Sound thresholds (KY-038 peak-to-peak amplitude, after filtering) ---
#define SOUND_THRESH_HIGH   200
#define SOUND_THRESH_LOW    100

// --- Debounce: consecutive readings to confirm state change ---
#define DEBOUNCE_COUNT      3

typedef enum {
    ALERT_OFF = 0,
    ALERT_ON  = 1
} AlertState_t;

// Synchronization primitives
extern SemaphoreHandle_t xSemNewData;   // binary semaphore: Acquisition -> Conditioning
extern SemaphoreHandle_t xSensorMutex;  // mutex: protects all shared variables

void         SensorData_Init(void);

// --- Temperature (digital sensor) ---
void         SensorData_SetTempReading(float temp, bool valid);
float        SensorData_GetTemperature(void);       // raw value
bool         SensorData_IsTempValid(void);
void         SensorData_SetTempFiltered(float val);
float        SensorData_GetTempFiltered(void);       // after filter pipeline
AlertState_t SensorData_GetTempAlert(void);
void         SensorData_SetTempAlert(AlertState_t state);
uint8_t      SensorData_GetTempDebounce(void);
void         SensorData_SetTempDebounce(uint8_t count);

// --- Sound (analog sensor) ---
void         SensorData_SetSoundReading(int value);
int          SensorData_GetSoundLevel(void);         // raw value
void         SensorData_SetSoundFiltered(float val);
float        SensorData_GetSoundFiltered(void);       // after filter pipeline
AlertState_t SensorData_GetSoundAlert(void);
void         SensorData_SetSoundAlert(AlertState_t state);
uint8_t      SensorData_GetSoundDebounce(void);
void         SensorData_SetSoundDebounce(uint8_t count);
