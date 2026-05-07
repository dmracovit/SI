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
#include "LdrSensor.h"
#include "LedPwmDriver.h"
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
    LdrSensor_Init();
    LedPwmDriver_Init();
    AppState_Init();

    printf("\n>>> Boot: Lab 10 - PID brightness regulator\n");
    printf("    LDR analog in : A0 (KY-018 module)\n");
    printf("    LED pwm out   : pin %d (Timer 2, FreeRTOS-safe)\n", LED_PWM_PIN);
    printf("    LCD I2C addr  : 0x%02X\n", LCD_ADDR);
    printf("    Init gains    : Kp=%.2f Ki=%.2f Kd=%.3f\n",
           (double)DEFAULT_KP, (double)DEFAULT_KI, (double)DEFAULT_KD);
    printf("    Init setpoint : %.1f %%\n", (double)DEFAULT_SETPOINT);
    printf("    LDR smoothing : EMA alpha=%.2f\n", (double)LDR_EMA_ALPHA);
    printf("    Type 'help' for STDIO command list.\n\n");

    LcdDisplay_PrintAt(0, 0, "Booting Lab10...");
    LcdDisplay_PrintAt(0, 1, "PID brightness  ");

    xTaskCreate(TaskSensor_Task,    "Sens", 192, NULL, 2, NULL);
    xTaskCreate(TaskControl_Task,   "Ctrl", 256, NULL, 3, NULL);
    xTaskCreate(TaskReporter_Task,  "Rprt", 256, NULL, 1, NULL);
    xTaskCreate(TaskCmd_Task,       "Cmd",  192, NULL, 1, NULL);
    xTaskCreate(TaskHeartbeat_Task, "Hb",   128, NULL, 1, NULL);

    // Arduino_FreeRTOS auto-starts scheduler after setup() returns.
}

void loop() { /* not used — FreeRTOS scheduler runs tasks */ }
