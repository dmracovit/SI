#ifndef CODE_LOCK_H
#define CODE_LOCK_H

#include <Arduino.h>

#define SECRET_CODE  "1234"
#define ADMIN_CODE   "0000"
#define CODE_LENGTH  4

void CodeLockInit(void);
bool CodeLockCheck(const char *code);
bool CodeLockIsAdmin(const char *code);

#endif
