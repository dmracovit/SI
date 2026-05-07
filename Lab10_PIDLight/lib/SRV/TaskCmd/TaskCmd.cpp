#include "TaskCmd.h"
#include "AppState.h"
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static char  rxBuf[32];
static uint8_t rxIdx = 0;

static void readSerialChars(void)
{
    while (Serial.available() > 0) {
        int c = Serial.read();
        if (c < 0) break;
        char ch = (char)c;
        putchar(ch);  // STDIO echo

        if (ch == '\n' || ch == '\r') {
            if (rxIdx > 0) {
                rxBuf[rxIdx] = '\0';
                for (uint8_t i = 0; i < rxIdx; i++)
                    rxBuf[i] = (char)tolower((unsigned char)rxBuf[i]);
                AppState_SetSerialCmd(rxBuf);
                rxIdx = 0;
            }
        } else if (rxIdx < sizeof(rxBuf) - 1) {
            rxBuf[rxIdx++] = ch;
        }
    }
}

static void printHelp(void)
{
    printf("\n>>> PID tuning commands:\n");
    printf("    set <0..100>  light setpoint (%%)\n");
    printf("    kp  <val>     proportional gain\n");
    printf("    ki  <val>     integral gain\n");
    printf("    kd  <val>     derivative gain\n");
    printf("    status        dump live values\n");
    printf("    help          show this list\n\n");
}

static void printStatus(void)
{
    printf("\n[live state]\n");
    printf("  light    : %.2f %%\n", AppState_GetLight());
    printf("  setpoint : %.2f %%\n", AppState_GetSetPoint());
    printf("  Kp/Ki/Kd : %.3f / %.3f / %.3f\n",
           AppState_GetKp(), AppState_GetKi(), AppState_GetKd());
    printf("  PWM out  : %u / 255\n", AppState_GetPwmOutput());
    printf("\n");
}

static void executeCommand(const char *line)
{
    float val;
    char  word[16] = {0};

    if (sscanf(line, "set %f", &val) == 1) {
        AppState_SetSetPoint(val);
        printf("[ok] setpoint -> %.2f %%\n", AppState_GetSetPoint());
        return;
    }
    if (sscanf(line, "kp %f", &val) == 1) {
        AppState_SetKp(val);
        printf("[ok] Kp -> %.4f\n", AppState_GetKp());
        return;
    }
    if (sscanf(line, "ki %f", &val) == 1) {
        AppState_SetKi(val);
        printf("[ok] Ki -> %.4f\n", AppState_GetKi());
        return;
    }
    if (sscanf(line, "kd %f", &val) == 1) {
        AppState_SetKd(val);
        printf("[ok] Kd -> %.4f\n", AppState_GetKd());
        return;
    }

    if (sscanf(line, "%15s", word) != 1) return;

    if      (strcmp(word, "status") == 0) printStatus();
    else if (strcmp(word, "help")   == 0) printHelp();
    else printf("[?] '%s' — type 'help'\n", line);
}

void TaskCmd_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;) {
        readSerialChars();
        if (AppState_HasSerialCmd()) {
            char raw[32];
            AppState_GetSerialCmd(raw, sizeof(raw));
            executeCommand(raw);
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CMD_TASK_PERIOD_MS));
    }
}
