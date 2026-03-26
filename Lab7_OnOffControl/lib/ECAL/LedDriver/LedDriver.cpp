#include "LedDriver.h"
#include "GpioDriver.h"

void LedDriver_Init(void)
{
    GpioDriver_PinMode(LED_PIN, OUTPUT);
    GpioDriver_Write(LED_PIN, LOW);
}

void LedDriver_On(void)  { GpioDriver_Write(LED_PIN, HIGH); }
void LedDriver_Off(void) { GpioDriver_Write(LED_PIN, LOW);  }

void LedDriver_Set(bool on)
{
    GpioDriver_Write(LED_PIN, on ? HIGH : LOW);
}
