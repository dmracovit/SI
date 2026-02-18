#include "SerialIO.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int uartPutchar(char c, FILE *stream)
{
    Serial.write(c);
    return 0;
}

static int uartGetchar(FILE *stream)
{
    if (!Serial.available()) return _FDEV_EOF;
    return Serial.read();
}

static FILE uartStdout;
static FILE uartStdin;

static char lineBuffer[INPUT_BUFFER_SIZE];
static uint8_t lineIndex = 0;

void SerialIoInit(void)
{
    Serial.begin(SERIAL_BAUD);
    while (!Serial)
    {
        ;
    }

    fdev_setup_stream(&uartStdout, uartPutchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &uartStdout;

    fdev_setup_stream(&uartStdin, NULL, uartGetchar, _FDEV_SETUP_READ);
    stdin = &uartStdin;

}

void SerialPrint(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

int SerialReadLine(char *buffer, int maxLength)
{
    int c;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n' || c == '\r')
        {
            putchar('\n');
            if (lineIndex > 0)
            {
                lineBuffer[lineIndex] = '\0';
                strncpy(buffer, lineBuffer, maxLength - 1);
                buffer[maxLength - 1] = '\0';
                int len = lineIndex;
                lineIndex = 0;
                return len;
            }
        }
        else if (lineIndex < INPUT_BUFFER_SIZE - 1)
        {
            putchar(c);
            lineBuffer[lineIndex++] = (char)c;
        }
    }
    return 0;
}
