#include "CodeLock.h"
#include <string.h>

void CodeLockInit(void)
{
    // Nothing to initialize for now
}

bool CodeLockCheck(const char *code)
{
    return strncmp(code, SECRET_CODE, CODE_LENGTH) == 0;
}

bool CodeLockIsAdmin(const char *code)
{
    return strncmp(code, ADMIN_CODE, CODE_LENGTH) == 0;
}
