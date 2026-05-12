#include "TaskCmd.h"
#include "AppState.h"
#include "FsmCore.h"
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static char rxBuf[32];
static uint8_t rxIdx = 0;

static void readSerialChars(void)
{
    while (Serial.available() > 0) {
        int c = Serial.read();
        if (c < 0) break;
        char ch = (char)c;
        putchar(ch);

        if (ch == '\n' || ch == '\r') {
            if (rxIdx > 0) {
                rxBuf[rxIdx] = '\0';
                for (uint8_t i = 0; i < rxIdx; i++)
                    rxBuf[i] = (char)tolower((unsigned char)rxBuf[i]);
                AppState_PushCmd(rxBuf);
                rxIdx = 0;
            }
        } else if (rxIdx < sizeof(rxBuf) - 1) {
            rxBuf[rxIdx++] = ch;
        }
    }
}

static void printHelp(void)
{
    printf("\n[FSM commands]\n");
    printf("  short    inject a short-press event\n");
    printf("  long     inject a long-press event\n");
    printf("  status   print current state & counter\n");
    printf("  help     show this list\n\n");
}

static void printStatus(void)
{
    const FsmState_t st = FsmCore_GetState();
    printf("[status] state=%s  transitions=%lu\n",
           FsmCore_StateName(st),
           (unsigned long)AppState_GetTransitions());
}

static void dispatch(const char *line)
{
    char word[16] = {0};
    if (sscanf(line, "%15s", word) != 1) return;

    if      (!strcmp(word, "help"))   printHelp();
    else if (!strcmp(word, "status")) printStatus();
    else if (!strcmp(word, "short"))  FsmCore_Step(FSM_EV_SHORT_PRESS);
    else if (!strcmp(word, "long"))   FsmCore_Step(FSM_EV_LONG_PRESS);
    else                              printf("[?] '%s' — type 'help'\n", line);
}

void TaskCmd_Task(void *pvParameters)
{
    (void)pvParameters;
    TickType_t cycleStart = xTaskGetTickCount();

    for (;;) {
        readSerialChars();
        if (AppState_HasCmd()) {
            char line[32];
            AppState_PopCmd(line, sizeof(line));
            dispatch(line);
        }
        vTaskDelayUntil(&cycleStart, pdMS_TO_TICKS(CMD_PERIOD_MS));
    }
}
