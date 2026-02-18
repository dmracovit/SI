#include "KeypadIO.h"
#include "LcdIO.h"
#include <Keypad.h>
#include <stdio.h>

static const char keyMap[KEYPAD_ROWS][KEYPAD_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

static uint8_t rowPins[KEYPAD_ROWS] = {2, 3, 4, 5};
static uint8_t colPins[KEYPAD_COLS] = {6, 7, 8, 9};

static Keypad keypad = Keypad(
    makeKeymap(keyMap),
    rowPins,
    colPins,
    KEYPAD_ROWS,
    KEYPAD_COLS
);

static int keypadGetchar(FILE *stream)
{
    char key = NO_KEY;
    while (key == NO_KEY)
    {
        key = keypad.getKey();
    }

    if (key == KEY_SUBMIT)
    {
        return '\n';   // scanf treats \n as delimiter
    }
    if (key == KEY_CLEAR)
    {
        return '\b';   // backspace — ignored by scanf, caller handles display
    }

    putchar('*');      // echo mask to LCD via stdout
    return key;
}

static FILE keypadStdin;

void KeypadIoInit(void)
{
    fdev_setup_stream(&keypadStdin, NULL, keypadGetchar, _FDEV_SETUP_READ);
    stdin = &keypadStdin;
}

int KeypadReadCode(char *buffer, int maxLen)
{
    LcdSetCursor(0, 1);

    char fmt[8];
    sprintf(fmt, "%%%ds", maxLen);   // build format string e.g. "%4s"

    int result = scanf(fmt, buffer); // reads from keypad stdin until '\n' (#)

    return (result == 1) ? strlen(buffer) : 0;
}
