#include "TaskInput.h"
#include "AppState.h"
#include "JoystickDriver.h"
#include <Arduino.h>
#include <string.h>
#include <stdio.h>

static char serialBuf[32];
static uint8_t serialIdx = 0;

// Uses getchar() (STDIO) for reading, putchar() for echo
static void processSerialInput(void)
{
    while (Serial.available() > 0) {
        int ch = getchar();              // STDIO input
        if (ch == EOF) break;

        char c = (char)ch;
        putchar(c);                      // STDIO echo

        if (c == '\n' || c == '\r') {
            if (serialIdx > 0) {
                serialBuf[serialIdx] = '\0';
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
        // --- Joystick press: toggle relay (power to servo) ---
        if (JoystickDriver_WasPressed()) {
            bool current = AppState_GetRelayCmd();
            AppState_SetRelayCmd(!current);
            printf("[INPUT] Relay toggle -> %s\n", !current ? "ON" : "OFF");
        }

        // --- Joystick angle: servo follows joystick direction ---
        if (JoystickDriver_IsActive()) {
            float angle = JoystickDriver_GetAngle();
            AppState_SetTarget(angle);
        }

        // --- Serial commands (sscanf) ---
        processSerialInput();
        if (AppState_HasSerialCmd()) {
            char raw[32];
            AppState_GetSerialCmd(raw, sizeof(raw));

            float val;
            char cmd[16];

            // "set 90" → set servo target
            if (sscanf(raw, "set %f", &val) == 1) {
                AppState_SetTarget(val);
                printf("[INPUT] Servo target: %.1f\n", AppState_GetTarget());
            }
            else if (sscanf(raw, "%15s", cmd) == 1) {
                if (strcmp(cmd, "on") == 0) {
                    AppState_SetRelayCmd(true);
                    printf("[INPUT] Relay: ON\n");
                } else if (strcmp(cmd, "off") == 0) {
                    AppState_SetRelayCmd(false);
                    printf("[INPUT] Relay: OFF\n");
                } else if (strcmp(cmd, "center") == 0) {
                    AppState_SetTarget(90.0f);
                    printf("[INPUT] Servo: center (90)\n");
                } else if (strcmp(cmd, "status") == 0) {
                    printf("[STATUS] Relay:%s Target:%.1f Actual:%.1f\n",
                           AppState_GetRelayState() ? "ON" : "OFF",
                           AppState_GetTarget(), AppState_GetActual());
                } else {
                    printf("[INPUT] Unknown: '%s'. Use: set <angle>, on, off, center, status\n", raw);
                }
            }
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(INPUT_TASK_PERIOD_MS));
    }
}
