#ifndef SERIAL_IO_H
#define SERIAL_IO_H

/* ECAL - SerialIO
 * Redirects stdout → Serial TX and stdin → Serial RX via fdev_setup_stream.
 * After SerialIoInit(), use printf() for output and scanf()/getchar() for input.
 * User code must NOT call Serial.print() directly.
 */

#include <Arduino.h>

#define SERIAL_BAUD 9600

void SerialIoInit(void);

#endif
