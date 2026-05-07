#include "AppState.h"
#include <string.h>

SemaphoreHandle_t xAppMutex = NULL;

static float   gLight    = 0.0f;
static float   gSetPoint = DEFAULT_SETPOINT;
static float   gKp       = DEFAULT_KP;
static float   gKi       = DEFAULT_KI;
static float   gKd       = DEFAULT_KD;
static uint8_t gPwm      = 0;

static char  gSerialCmd[32] = {0};
static bool  gHasSerialCmd  = false;

void AppState_Init(void) { xAppMutex = xSemaphoreCreateMutex(); }

#define LOCK()    xSemaphoreTake(xAppMutex, portMAX_DELAY)
#define UNLOCK()  xSemaphoreGive(xAppMutex)

void  AppState_SetLight(float p)    { LOCK(); gLight = p;    UNLOCK(); }
float AppState_GetLight(void)       { LOCK(); float v = gLight;    UNLOCK(); return v; }

void  AppState_SetSetPoint(float sp){ if (sp < 0) sp = 0; if (sp > 100) sp = 100;
                                      LOCK(); gSetPoint = sp; UNLOCK(); }
float AppState_GetSetPoint(void)    { LOCK(); float v = gSetPoint; UNLOCK(); return v; }

void  AppState_SetKp(float v)       { LOCK(); gKp = v; UNLOCK(); }
void  AppState_SetKi(float v)       { LOCK(); gKi = v; UNLOCK(); }
void  AppState_SetKd(float v)       { LOCK(); gKd = v; UNLOCK(); }
float AppState_GetKp(void)          { LOCK(); float v = gKp; UNLOCK(); return v; }
float AppState_GetKi(void)          { LOCK(); float v = gKi; UNLOCK(); return v; }
float AppState_GetKd(void)          { LOCK(); float v = gKd; UNLOCK(); return v; }

void    AppState_SetPwmOutput(uint8_t d) { LOCK(); gPwm = d; UNLOCK(); }
uint8_t AppState_GetPwmOutput(void)      { LOCK(); uint8_t v = gPwm; UNLOCK(); return v; }

void AppState_SetSerialCmd(const char *cmd)
{
    LOCK();
    strncpy(gSerialCmd, cmd, sizeof(gSerialCmd) - 1);
    gSerialCmd[sizeof(gSerialCmd) - 1] = '\0';
    gHasSerialCmd = true;
    UNLOCK();
}

bool AppState_HasSerialCmd(void)
{
    LOCK(); bool v = gHasSerialCmd; UNLOCK(); return v;
}

void AppState_GetSerialCmd(char *buf, size_t len)
{
    LOCK();
    strncpy(buf, gSerialCmd, len - 1);
    buf[len - 1] = '\0';
    gHasSerialCmd = false;
    UNLOCK();
}
