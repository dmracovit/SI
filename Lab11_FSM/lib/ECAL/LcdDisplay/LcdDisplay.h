#pragma once
#include <Arduino.h>

#define LCD_ADDR    0x27
#define LCD_COLS    16
#define LCD_ROWS    2
#define LCD_SDA_PIN 21
#define LCD_SCL_PIN 22

void LcdDisplay_Init(void);
void LcdDisplay_Clear(void);
void LcdDisplay_PrintAt(uint8_t col, uint8_t row, const char *text);
