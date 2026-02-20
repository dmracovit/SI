#include <Arduino.h>
#include <stdio.h>
#include "LedControl.h"
#include "LcdIO.h"
#include "KeypadIO.h"
#include "CodeLock.h"

#define RESULT_DISPLAY_MS 3000
#define BLINK_MS          2000
#define BLINK_COUNT       2

char codeBuffer[CODE_LENGTH + 1];

void setup()
{
    LedInit();
    LcdIoInit();      // stdout -> LCD
    KeypadIoInit();   // stdin  -> Keypad
    CodeLockInit();

    // printf goes to LCD via stdout redirect
    printf("    Lab 1.2\n");
    printf("Code Lock System");
    delay(RESULT_DISPLAY_MS);
}

void loop()
{
    LcdClear();
    printf("Enter code:\n");

    KeypadReadCode(codeBuffer, CODE_LENGTH);

    LcdClear();
    if (CodeLockIsAdmin(codeBuffer))
    {
        printf("  Admin Mode!\n");
        for (int i = 0; i < BLINK_COUNT; i++)
        {
            LedRedOn();
            delay(BLINK_MS);
            LedRedOff();
            delay(200);
        }
        for (int i = 0; i < BLINK_COUNT; i++)
        {
            LedGreenOn();
            delay(BLINK_MS);
            LedGreenOff();
            delay(200);
        }
    }
    else if (CodeLockCheck(codeBuffer))
    {
        printf("Access Granted!\n");
        LedGreenOn();
        delay(RESULT_DISPLAY_MS);
        LedGreenOff();
    }
    else
    {
        printf("Access Denied!\n");
        LedRedOn();
        delay(RESULT_DISPLAY_MS);
        LedRedOff();
    }
}
