#include "SoundSensor.h"
#include "GpioDriver.h"

#define SAMPLE_COUNT 50

void SoundSensor_Init(void)
{
}

int SoundSensor_Read(void)
{
    // Measure peak-to-peak amplitude over a short window.
    // Louder sounds = larger amplitude.
    int sampleMin = 4095;
    int sampleMax = 0;

    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        int val = GpioDriver_AnalogRead(SOUND_SENSOR_PIN);
        if (val > sampleMax)
            sampleMax = val;
        if (val < sampleMin)
            sampleMin = val;
    }

    return sampleMax - sampleMin;
}
