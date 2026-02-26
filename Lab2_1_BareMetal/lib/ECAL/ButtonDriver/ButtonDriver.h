#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

/* ECAL - ButtonDriver
 * Reads the button connected to D2 with INPUT_PULLUP.
 * Compatible with KY-023 joystick SW pin.
 * Active LOW: pin reads LOW when pressed, HIGH when released.
 */

#include <Arduino.h>

#define BUTTON_PIN 2

void ButtonDriver_Init(void);
bool ButtonDriver_Read(void); // returns true when pressed

#endif
