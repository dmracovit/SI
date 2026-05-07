#include <Arduino.h>
#include <Wire.h>
#include "SerialIO.h"
#include "LcdDisplay.h"

static void scanI2cBus(void)
{
    Wire.begin();
    printf("[I2C] scanning bus...\n");
    uint8_t found = 0;
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {
            printf("[I2C] device at 0x%02X\n", addr);
            found++;
        }
    }
    if (!found) printf("[I2C] no devices found — check SDA=20, SCL=21, 5V, GND\n");
}
#include "DhtSensor.h"
#include "RelayDriver.h"
#include "AppState.h"
#include "TaskSensor.h"
#include "TaskControl.h"
#include "TaskCmd.h"
#include "TaskReporter.h"
#include "TaskHeartbeat.h"

void setup()
{
    SerialIoInit();
    scanI2cBus();
    LcdDisplay_Init();
    DhtSensor_Init();
    RelayDriver_Init();
    AppState_Init();

    printf("\n>>> Boot: Lab 9 - Hysteretic thermal control\n");
    printf("    DHT22 data line  : pin %d\n", DHT_PIN);
    printf("    Relay control    : pin %d (active-LOW, drives heater)\n", RELAY_PIN);
    printf("    LCD I2C addr     : 0x%02X\n", LCD_ADDR);
    printf("    Init setpoint    : %d.%d C\n",
           (int)DEFAULT_SETPOINT, (int)((DEFAULT_SETPOINT - (int)DEFAULT_SETPOINT) * 10));
    printf("    Init hysteresis  : +/- %d.%d C\n",
           (int)DEFAULT_HYSTERESIS, (int)((DEFAULT_HYSTERESIS - (int)DEFAULT_HYSTERESIS) * 10));
    printf("    Type 'help' for STDIO command list.\n\n");

    LcdDisplay_PrintAt(0, 0, "Booting Lab9... ");
    LcdDisplay_PrintAt(0, 1, "Thermal regulat.");

    xTaskCreate(TaskSensor_Task,    "Sens", 192, NULL, 2, NULL);
    xTaskCreate(TaskControl_Task,   "Ctrl", 192, NULL, 3, NULL);
    xTaskCreate(TaskReporter_Task,  "Rprt", 256, NULL, 1, NULL);
    xTaskCreate(TaskCmd_Task,       "Cmd",  256, NULL, 1, NULL);
    xTaskCreate(TaskHeartbeat_Task, "Hb",   128, NULL, 1, NULL);
}

void loop() { /* FreeRTOS scheduler handles tasks */ }
