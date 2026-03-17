#include "TaskAcquisition.h"
#include "TempSensor.h"
#include "SoundSensor.h"
#include "SensorData.h"

// Period must be >= DS18B20 9-bit conversion time (93.75ms)
#define ACQUISITION_PERIOD_MS 100

void TaskAcquisition_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        // --- Digital sensor: DS18B20 (non-blocking read) ---
        float temp  = TempSensor_Read();
        bool  valid = TempSensor_IsValid();
        SensorData_SetTempReading(temp, valid);

        // --- Analog sensor: KY-038 sound (ADC read) ---
        int sound = SoundSensor_Read();
        SensorData_SetSoundReading(sound);

        // Signal Conditioning task that new data is ready
        xSemaphoreGive(xSemNewData);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ACQUISITION_PERIOD_MS));
    }
}
