#include "LedPwmDriver.h"
#include "GpioDriver.h"

void LedPwmDriver_Init(void)
{
    GpioDriver_PinMode(LED_PWM_PIN, OUTPUT);
    analogWrite(LED_PWM_PIN, 0);
}

void LedPwmDriver_SetDuty(uint8_t duty)
{
    analogWrite(LED_PWM_PIN, duty);   // 8-bit PWM on AVR
}
