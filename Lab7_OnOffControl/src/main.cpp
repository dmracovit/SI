#include <Arduino.h>
#include <Wire.h>
#include "SerialIO.h"
#include "LcdDisplay.h"
#include "LedDriver.h"
#include "RelayDriver.h"
#include "JoystickDriver.h"
#include "AppState.h"
#include "TaskInput.h"
#include "TaskConditioning.h"
#include "TaskReporter.h"

void setup()
{
    SerialIoInit();

    // I2C scan to find LCD address
    Wire.begin(21, 22);
    printf("\n[I2C SCAN] Scanning...\n");
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            printf("[I2C SCAN] Found device at 0x%02X\n", addr);
        }
    }
    printf("[I2C SCAN] Done.\n\n");

    LcdDisplay_Init();
    LedDriver_Init();
    RelayDriver_Init();
    JoystickDriver_Init();
    AppState_Init();

    printf("\n======================================\n");
    printf("  Lab 7 - ON-OFF Control + Hysteresis \n");
    printf("======================================\n");
    printf("Relay     : GPIO%d\n",   RELAY_PIN);
    printf("Joystick  : SW=GPIO%d\n", JOYSTICK_SWITCH_PIN);
    printf("LED       : GPIO%d\n",   LED_PIN);
    printf("LCD       : I2C 0x%02X\n", LCD_ADDR);
    printf("Debounce  : %dms\n",     DEBOUNCE_TIME_MS);
    printf("Validation: %dms\n",     VALIDATION_TIME_MS);
    printf("--------------------------------------\n");
    printf("Serial commands: on, off, toggle, status\n");
    printf("======================================\n\n");

    LcdDisplay_PrintAt(0, 0, "Lab7 ON-OFF Ctrl");
    LcdDisplay_PrintAt(0, 1, "Starting...");
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Task priorities: Input=3, Conditioning=3, Reporter=2
    xTaskCreate(TaskInput_Task,        "Input",        4096, NULL, 3, NULL);
    xTaskCreate(TaskConditioning_Task, "Conditioning", 4096, NULL, 3, NULL);
    xTaskCreate(TaskReporter_Task,     "Reporter",     4096, NULL, 2, NULL);
}

void loop()
{
    vTaskDelay(portMAX_DELAY);
}
