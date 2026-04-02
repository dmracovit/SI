#include <Arduino.h>
#include "SerialIO.h"
#include "LcdDisplay.h"
#include "ServoDriver.h"
#include "RelayDriver.h"
#include "JoystickDriver.h"
#include "AppState.h"
#include "SignalFilter.h"
#include "TaskInput.h"
#include "TaskConditioning.h"
#include "TaskReporter.h"

void setup()
{
    SerialIoInit();
    LcdDisplay_Init();
    ServoDriver_Init();
    RelayDriver_Init();
    JoystickDriver_Init();
    AppState_Init();

    printf("\n=============================================\n");
    printf("  Lab 8 - Dual Actuator Control (100%%)      \n");
    printf("=============================================\n");
    printf("Binary  : Relay on GPIO%d\n",    RELAY_PIN);
    printf("Analog  : Servo on GPIO%d (PWM)\n", SERVO_PIN);
    printf("Joystick: X=GPIO%d Y=GPIO%d SW=GPIO%d\n",
           JOYSTICK_X_PIN, JOYSTICK_Y_PIN, JOYSTICK_SWITCH_PIN);
    printf("LCD     : I2C 0x%02X\n", LCD_ADDR);
    printf("Servo   : %.0f-%.0f deg, ramp %.1f deg/cycle\n",
           ANGLE_MIN, ANGLE_MAX, RAMP_STEP);
    printf("Filter  : Median(%d) + EMA(%.1f)\n", MEDIAN_WINDOW, EMA_ALPHA);
    printf("---------------------------------------------\n");
    printf("Joystick: rotate=servo angle, press=relay toggle\n");
    printf("Serial  : set <angle>, on, off, center, status\n");
    printf("=============================================\n\n");

    LcdDisplay_PrintAt(0, 0, "Lab8 DualActuatr");
    LcdDisplay_PrintAt(0, 1, "Starting...");
    vTaskDelay(pdMS_TO_TICKS(1000));

    xTaskCreate(TaskInput_Task,        "Input",        4096, NULL, 3, NULL);
    xTaskCreate(TaskConditioning_Task, "Conditioning", 4096, NULL, 3, NULL);
    xTaskCreate(TaskReporter_Task,     "Reporter",     4096, NULL, 2, NULL);
}

void loop()
{
    vTaskDelay(portMAX_DELAY);
}
