#include "DhtSensor.h"
#include <DHT.h>

static DHT dht;
static bool initialized = false;

void DhtSensor_Init(void)
{
    dht.setup(DHT_PIN, DHT::DHT22);
    initialized = true;
    delay(100);
}

bool DhtSensor_Read(float *temperature, float *humidity)
{
    if (!initialized) return false;
    float t = dht.getTemperature();
    float h = dht.getHumidity();
    if (isnan(t) || isnan(h) || dht.getStatus() != DHT::ERROR_NONE) return false;
    *temperature = t;
    *humidity    = h;
    return true;
}
