#include "SoundSensor.h"
#include "GpioDriver.h"

// Number of samples to capture for peak-to-peak amplitude measurement
#define SAMPLE_COUNT 50
// Sampling window ~5ms (50 samples at ~100us each)

void SoundSensor_Init(void)
{
    // GPIO34 is input-only on ESP32, no need for explicit pinMode
}

int SoundSensor_Read(void)
{
    // Measure peak-to-peak amplitude over a short window.
    // Louder sounds = larger amplitude.
    int sampleMin = 4095;
    int sampleMax = 0;

    for (int i = 0; i < SAMPLE_COUNT; i++) {
        int val = GpioDriver_AnalogRead(SOUND_SENSOR_PIN);
        if (val > sampleMax) sampleMax = val;
        if (val < sampleMin) sampleMin = val;
    }

    // Return peak-to-peak amplitude (0 = silence, higher = louder)
    return sampleMax - sampleMin;
}
