#include "LedControl.h"

void LedInit(void)
{
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);
    digitalWrite(LED_GREEN_PIN, LOW);
    digitalWrite(LED_RED_PIN, LOW);
}

void LedGreenOn(void)
{
    digitalWrite(LED_GREEN_PIN, HIGH);
}

void LedGreenOff(void)
{
    digitalWrite(LED_GREEN_PIN, LOW);
}

void LedRedOn(void)
{
    digitalWrite(LED_RED_PIN, HIGH);
}

void LedRedOff(void)
{
    digitalWrite(LED_RED_PIN, LOW);
}
