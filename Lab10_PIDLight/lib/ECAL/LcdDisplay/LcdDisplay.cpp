#include "LcdDisplay.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

static LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

void LcdDisplay_Init(void)
{
    Wire.begin();           // Mega default I2C: SDA=20, SCL=21
    lcd.init();             // marcoschwartz API
    lcd.backlight();
    lcd.clear();
}

void LcdDisplay_Clear(void) { lcd.clear(); }

void LcdDisplay_PrintAt(uint8_t col, uint8_t row, const char *text)
{
    lcd.setCursor(col, row);
    lcd.print(text);
}
