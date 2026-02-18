#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#include <Arduino.h>

#define SERIAL_BAUD 9600
#define INPUT_BUFFER_SIZE 32

void SerialIoInit(void);
void SerialPrint(const char *format, ...);
int SerialReadLine(char *buffer, int maxLength);

#endif
