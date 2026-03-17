#pragma once
#include <stdint.h>

// LCD 16x2 with I2C backpack
// Default I2C address: 0x27 (change to 0x3F if LCD doesn't respond)
// ESP32: SDA = GPIO21, SCL = GPIO22

void LcdDisplay_Init(void);
void LcdDisplay_Clear(void);
void LcdDisplay_SetCursor(uint8_t col, uint8_t row);
void LcdDisplay_Print(const char *str);
void LcdDisplay_PrintAt(uint8_t col, uint8_t row, const char *str);
