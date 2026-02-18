#include <Arduino.h>
#include <stdio.h>
#include "LedControl.h"
#include "LcdIO.h"
#include "KeypadIO.h"
#include "CodeLock.h"

#define RESULT_DISPLAY_MS 3000

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
    if (CodeLockCheck(codeBuffer))
    {
        printf("Access Granted!\n");
        LedGreenOn();
    }
    else
    {
        printf("Access Denied!\n");
        LedRedOn();
    }

    delay(RESULT_DISPLAY_MS);
    LedGreenOff();
    LedRedOff();
}
