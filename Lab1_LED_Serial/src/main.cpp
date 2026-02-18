#include <Arduino.h>
#include <stdio.h>
#include "SerialIO.h"
#include "CommandParser.h"
#include "LedControl.h"

char commandBuffer[32];

void setup()
{
    LedInit();
    SerialIoInit();
    CommandParserInit();

    printf("\n=================================\n");
    printf("Lab 1.1 - LED Control via Serial\n");
    printf("=================================\n");
    printf("Commands:\n");
    printf("  led on    - Turn LED on\n");
    printf("  led off   - Turn LED off\n");
    printf("  led blink - Blink LED every 500ms\n");
    printf("=================================\n\n");
    printf("System ready. Waiting for commands...\n");
}

void loop()
{
    int length = SerialReadLine(commandBuffer, sizeof(commandBuffer));
    if (length > 0)
    {
        CommandParserExecute(commandBuffer);
    }

    CommandParserUpdate();
}
