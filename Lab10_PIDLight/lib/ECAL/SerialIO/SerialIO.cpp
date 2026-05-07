#include "SerialIO.h"
#include <Arduino.h>
#include <stdio.h>

static int serialPutchar(char c, FILE *f)
{
    (void)f;
    Serial.write(c);
    return 0;
}

static int serialGetchar(FILE *f)
{
    (void)f;
    while (!Serial.available()) { /* spin */ }
    return Serial.read();
}

static FILE serialIO;

void SerialIoInit(void)
{
    Serial.begin(SERIAL_BAUD);
    fdev_setup_stream(&serialIO, serialPutchar, serialGetchar, _FDEV_SETUP_RW);
    stdout = &serialIO;
    stdin  = &serialIO;
}
