#include <Arduino.h>
#include "SerialIO.h"
#include "CommandParser.h"
#include "LedControl.h"

char commandBuffer[32];

void setup()
{
    LedInit();
    SerialIoInit();
    CommandParserInit();
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
