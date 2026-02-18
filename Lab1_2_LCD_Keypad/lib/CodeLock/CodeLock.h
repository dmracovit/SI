#ifndef CODE_LOCK_H
#define CODE_LOCK_H

#include <Arduino.h>

#define SECRET_CODE  "1234"
#define CODE_LENGTH  4

void CodeLockInit(void);
bool CodeLockCheck(const char *code);

#endif
