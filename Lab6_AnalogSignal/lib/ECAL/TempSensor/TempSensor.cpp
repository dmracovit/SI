#include "TempSensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

static OneWire           oneWire(TEMP_SENSOR_PIN);
static DallasTemperature sensors(&oneWire);
static bool              lastValid = false;

void TempSensor_Init(void)
{
    sensors.begin();
    sensors.setResolution(9);             // 9-bit: 93.75ms conversion time
    sensors.setWaitForConversion(false);  // non-blocking: requestTemperatures() returns immediately
    sensors.requestTemperatures();        // start first conversion
    delay(100);                           // wait for first conversion to complete before tasks start
}

float TempSensor_Read(void)
{
    // Read result of previous conversion, then start the next one.
    // This pattern ensures we never block inside a FreeRTOS task.
    float t = sensors.getTempCByIndex(0);
    lastValid = (t != DEVICE_DISCONNECTED_C);
    sensors.requestTemperatures();  // start next conversion (ready by next call ~100ms later)
    return t;
}

bool TempSensor_IsValid(void)
{
    return lastValid;
}
