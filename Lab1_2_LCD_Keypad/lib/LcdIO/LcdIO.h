#ifndef LCD_IO_H
#define LCD_IO_H

#include <Arduino.h>

#define LCD_ADDRESS 0x27
#define LCD_COLS    16
#define LCD_ROWS    2

void LcdIoInit(void);
void LcdClear(void);
void LcdSetCursor(uint8_t col, uint8_t row);

// After LcdIoInit(), printf() writes directly to LCD via stdout redirect

#endif
