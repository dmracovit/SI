#include "LedControl.h"

static bool ledState = false;

void LedInit(void)
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    ledState = false;
}

void LedOn(void)
{
    digitalWrite(LED_PIN, HIGH);
    ledState = true;
}

void LedOff(void)
{
    digitalWrite(LED_PIN, LOW);
    ledState = false;
}

void LedToggle(void)
{
    if (ledState)
    {
        LedOff();
    }
    else
    {
        LedOn();
    }
}

bool LedGetState(void)
{
    return ledState;
}
