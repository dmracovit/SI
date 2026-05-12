#include "LdrSensor.h"
#include "GpioDriver.h"

// LDR module wiring (KY-018-style): a fixed pull-up resistor sits between
// VCC and the analog pin, the photo-resistor between the pin and GND.
// Hence raw ADC counts rise when it is dark and fall in bright light, so
// we invert the value when scaling to percent (0 = dark, 100 = bright).
//
// Burst-sampling strategy: take a small sorted burst of N readings,
// discard the lowest and the highest, and average the middle (trimmed
// mean). This is more robust than a plain arithmetic mean against
// occasional ADC outliers caused by the 490 Hz PWM on pin 9 or by
// 50/60 Hz mains hum.

#define LDR_BURST_SIZE         9      // N samples per read
#define LDR_BURST_TRIM         2      // discard lowest 2 and highest 2
#define LDR_INTERSAMPLE_US     680    // ~6 ms total → covers ≥ 2 PWM cycles

static int16_t lastFiltered = 0;

static int16_t lcdSensorMedian(int16_t *buf, uint8_t n)
{
    // In-place insertion sort — small N, fast on AVR.
    for (uint8_t i = 1; i < n; ++i) {
        int16_t k = buf[i];
        int8_t  j = (int8_t)i - 1;
        while (j >= 0 && buf[j] > k) {
            buf[j + 1] = buf[j];
            --j;
        }
        buf[j + 1] = k;
    }
    return buf[n / 2];
}

void LdrSensor_Init(void)
{
    GpioDriver_PinMode(LDR_PIN, INPUT);
}

int LdrSensor_ReadRaw(void)
{
    int16_t burst[LDR_BURST_SIZE];

    // Collect raw samples spaced over a multi-millisecond window.
    for (uint8_t i = 0; i < LDR_BURST_SIZE; ++i) {
        burst[i] = (int16_t)GpioDriver_AnalogRead(LDR_PIN);
        if (i + 1 < LDR_BURST_SIZE) {
            delayMicroseconds(LDR_INTERSAMPLE_US);
        }
    }

    // Sort once, take a trimmed mean over the central span.
    (void)lcdSensorMedian(burst, LDR_BURST_SIZE);   // sort side-effect

    int32_t sum    = 0;
    uint8_t kept   = 0;
    for (uint8_t i = LDR_BURST_TRIM; i < LDR_BURST_SIZE - LDR_BURST_TRIM; ++i) {
        sum += burst[i];
        ++kept;
    }

    lastFiltered = (int16_t)((sum + (int32_t)(kept / 2)) / (int32_t)kept);
    return (int)lastFiltered;
}

float LdrSensor_ReadPercent(void)
{
    const int raw     = LdrSensor_ReadRaw();
    const float fpct  = 100.0f - ((float)raw * (100.0f / 1023.0f));
    if (fpct <   0.0f) return   0.0f;
    if (fpct > 100.0f) return 100.0f;
    return fpct;
}
