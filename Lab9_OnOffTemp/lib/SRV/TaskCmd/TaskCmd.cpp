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
    printf("\n>>> Available commands:\n");
    printf("    set  <C>   target temperature (range %.0f..%.0f)\n", SETPOINT_MIN, SETPOINT_MAX);
    printf("    hyst <C>   half-band of deadzone (range %.0f..%.0f)\n", HYST_MIN, HYST_MAX);
    printf("    status     dump live values\n");
    printf("    help       show this list\n\n");
}

static void printStatus(void)
{
    printf("\n[live state]\n");
    printf("  temperature : %.2f C  (valid=%d)\n",
           AppState_GetTemperature(), AppState_IsSensorValid() ? 1 : 0);
    printf("  humidity    : %.2f %%\n",  AppState_GetHumidity());
    printf("  setpoint    : %.2f C\n",   AppState_GetSetPoint());
    printf("  hysteresis  : +/- %.2f C\n", AppState_GetHysteresis());
    printf("  relay       : %s\n",       AppState_GetRelay() ? "ON (heating)" : "OFF (idle)");
    printf("\n");
}

static void executeCommand(const char *line)
{
    float val;
    char  word[16] = {0};

    if (sscanf(line, "set %f", &val) == 1) {
        AppState_SetSetPoint(val);
        printf("[ok] setpoint -> %.2f C\n", AppState_GetSetPoint());
        return;
    }
    if (sscanf(line, "hyst %f", &val) == 1) {
        AppState_SetHysteresis(val);
        printf("[ok] hysteresis -> %.2f C\n", AppState_GetHysteresis());
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
