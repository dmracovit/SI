#ifndef KEYPAD_IO_H
#define KEYPAD_IO_H

#include <Arduino.h>

#define KEYPAD_ROWS  4
#define KEYPAD_COLS  4
#define KEY_SUBMIT  '#'
#define KEY_CLEAR   '*'
#define CODE_MAX_LEN 16

void KeypadIoInit(void);
int  KeypadReadCode(char *buffer, int maxLen);

// After KeypadIoInit(), getchar() reads from keypad via stdin redirect

#endif
