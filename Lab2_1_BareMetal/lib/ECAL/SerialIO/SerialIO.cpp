#include "SerialIO.h"
#include <stdio.h>

static int uartPutchar(char c, FILE *stream)
{
    Serial.write(c);
    return 0;
}

static int uartGetchar(FILE *stream)
{
    while (!Serial.available()) { ; }
    int c = Serial.read();
    Serial.write(c);
    return c;
}

static FILE uartStdout;
static FILE uartStdin;

void SerialIoInit(void)
{
    Serial.begin(SERIAL_BAUD);
    while (!Serial) { ; }

    fdev_setup_stream(&uartStdout, uartPutchar, NULL,        _FDEV_SETUP_WRITE);
    fdev_setup_stream(&uartStdin,  NULL,        uartGetchar, _FDEV_SETUP_READ);

    stdout = &uartStdout;
    stdin  = &uartStdin;
}
