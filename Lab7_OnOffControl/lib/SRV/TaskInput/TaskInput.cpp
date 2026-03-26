#include "TaskInput.h"
#include "AppState.h"
#include "JoystickDriver.h"
#include <Arduino.h>
#include <string.h>

// Read serial input character by character, build command on newline
static char serialBuf[32];
static uint8_t serialIdx = 0;

static void processSerialInput(void)
{
    while (Serial.available() > 0) {
        char c = (char)Serial.read();

        // Echo character back
        Serial.write(c);

        if (c == '\n' || c == '\r') {
            if (serialIdx > 0) {
                serialBuf[serialIdx] = '\0';

                // Convert to lowercase
                for (uint8_t i = 0; i < serialIdx; i++) {
                    if (serialBuf[i] >= 'A' && serialBuf[i] <= 'Z')
                        serialBuf[i] += 32;
                }

                AppState_SetSerialCmd(serialBuf);
                serialIdx = 0;
            }
        } else if (serialIdx < sizeof(serialBuf) - 1) {
            serialBuf[serialIdx++] = c;
        }
    }
}

void TaskInput_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        CmdState_t current = AppState_GetCommand();

        // --- Joystick switch: toggle on press ---
        if (JoystickDriver_WasPressed()) {
            CmdState_t next = (current == CMD_ON) ? CMD_OFF : CMD_ON;
            AppState_SetCommand(next);
            printf("[INPUT] Joystick toggle -> %s\n", next == CMD_ON ? "ON" : "OFF");
        }

        // --- Serial commands (parsed with sscanf — STDIO) ---
        processSerialInput();
        if (AppState_HasSerialCmd()) {
            char raw[32];
            AppState_GetSerialCmd(raw, sizeof(raw));

            // Use sscanf to parse the command string
            char cmd[32];
            if (sscanf(raw, "%31s", cmd) == 1) {
                if (strcmp(cmd, "on") == 0) {
                    AppState_SetCommand(CMD_ON);
                    printf("[INPUT] Serial: ON\n");
                } else if (strcmp(cmd, "off") == 0) {
                    AppState_SetCommand(CMD_OFF);
                    printf("[INPUT] Serial: OFF\n");
                } else if (strcmp(cmd, "toggle") == 0) {
                    CmdState_t next = (current == CMD_ON) ? CMD_OFF : CMD_ON;
                    AppState_SetCommand(next);
                    printf("[INPUT] Serial: toggle -> %s\n", next == CMD_ON ? "ON" : "OFF");
                } else if (strcmp(cmd, "status") == 0) {
                    printf("[STATUS] Cmd:%s Relay:%s Toggles:%lu\n",
                           AppState_GetCommand() == CMD_ON ? "ON" : "OFF",
                           AppState_GetRelayState() ? "ON" : "OFF",
                           (unsigned long)AppState_GetToggleCount());
                } else {
                    printf("[INPUT] Unknown command: '%s'\n", cmd);
                }
            }
        }

        // Signal display update
        xSemaphoreGive(xSemDisplayUpdate);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(INPUT_TASK_PERIOD_MS));
    }
}
