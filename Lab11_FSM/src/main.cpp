#include <Arduino.h>
#include "SerialIO.h"
#include "LcdDisplay.h"
#include "LedDriver.h"
#include "JoystickDriver.h"
#include "AppState.h"
#include "FsmCore.h"
#include "TaskFsm.h"
#include "TaskDisplay.h"
#include "TaskCmd.h"

void setup()
{
    SerialIoInit();
    printf("\n>>> Lab 11 booting...\n");

    LedDriver_Init();
    LedDriver_On();  delay(100);
    LedDriver_Off();
    printf("    LED ok (GPIO%d)\n", LED_PIN);

    JoystickDriver_Init();
    AppState_Init();
    FsmCore_Init();
    printf("    FSM ok\n");

    LcdDisplay_Init();
    LcdDisplay_PrintAt(0, 0, "Lab11 FSM       ");
    LcdDisplay_PrintAt(0, 1, "Ready.          ");
    printf("    LCD ok\n");

    printf("    Type 'help' for serial commands.\n\n");

    xTaskCreate(TaskFsm_Task,     "Fsm",  4096, NULL, 3, NULL);
    xTaskCreate(TaskDisplay_Task, "Disp", 4096, NULL, 2, NULL);
    xTaskCreate(TaskCmd_Task,     "Cmd",  4096, NULL, 1, NULL);
}

void loop()
{
    vTaskDelay(portMAX_DELAY);
}
