#include "LdrSensor.h"
#include "GpioDriver.h"

// KY-018 module: fixed resistor between VCC and signal, LDR between signal
// and GND. Raw ADC goes UP in dark, DOWN in bright → invert when scaling.
// Multi-sample averaging filters PWM chop and mains hum.
#define STAB_SAMPLES   16
#define SAMPLE_GAP_US  700

static int lastRaw = 0;

void LdrSensor_Init(void)
{
    GpioDriver_PinMode(LDR_PIN, INPUT);
}

int LdrSensor_ReadRaw(void)
{
    long sum = 0;
    for (int i = 0; i < STAB_SAMPLES; i++) {
        sum += GpioDriver_AnalogRead(LDR_PIN);
        if (i < STAB_SAMPLES - 1) delayMicroseconds(SAMPLE_GAP_US);
    }
    lastRaw = (int)((sum + STAB_SAMPLES / 2) / STAB_SAMPLES);
    return lastRaw;
}

float LdrSensor_ReadPercent(void)
{
    int raw = LdrSensor_ReadRaw();
    // invert: high raw (dark) → 0%, low raw (bright) → 100%
    float pct = 100.0f - ((float)raw * (100.0f / 1023.0f));
    if (pct < 0)   pct = 0;
    if (pct > 100) pct = 100;
    return pct;
}
