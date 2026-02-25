#include "SerialIO.h"
#include <stdio.h>

static int uartPutchar(char c, FILE *stream)
{
    Serial.write(c);
    return 0;
}

static FILE uartStdout;

void SerialIoInit(void)
{
    Serial.begin(SERIAL_BAUD);
    while (!Serial) { ; }

    fdev_setup_stream(&uartStdout, uartPutchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &uartStdout;
}
