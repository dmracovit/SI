#include "LcdIO.h"
#include <LiquidCrystal_I2C.h>
#include <stdio.h>

static LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

static uint8_t lcdCol = 0;
static uint8_t lcdRow = 0;

static int lcdPutchar(char c, FILE *stream)
{
    if (c == '\f')
    {
        lcd.clear();
        lcdCol = 0;
        lcdRow = 0;
        lcd.setCursor(lcdCol, lcdRow);
    }
    else if (c == '\n')
    {
        lcdRow++;
        lcdCol = 0;
        if (lcdRow >= LCD_ROWS)
        {
            lcdRow = 0;
        }
        lcd.setCursor(lcdCol, lcdRow);
    }
    else
    {
        lcd.write(c);
        lcdCol++;
        if (lcdCol >= LCD_COLS)
        {
            lcdCol = 0;
            lcdRow++;
            if (lcdRow >= LCD_ROWS)
            {
                lcdRow = 0;
            }
            lcd.setCursor(lcdCol, lcdRow);
        }
    }
    return 0;
}

static FILE lcdStdout;

void LcdIoInit(void)
{
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcdCol = 0;
    lcdRow = 0;

    fdev_setup_stream(&lcdStdout, lcdPutchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &lcdStdout;
}

void LcdClear(void)
{
    lcd.clear();
    lcdCol = 0;
    lcdRow = 0;
}

void LcdSetCursor(uint8_t col, uint8_t row)
{
    lcdCol = col;
    lcdRow = row;
    lcd.setCursor(col, row);
}
