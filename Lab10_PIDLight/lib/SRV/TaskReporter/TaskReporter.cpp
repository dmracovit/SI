#include "TaskReporter.h"
#include "AppState.h"
#include "LcdDisplay.h"

// LCD page rotates every 3 reporter cycles.
#define LCD_PAGE_HOLD 3

void TaskReporter_Task(void *pvParameters)
{
    vTaskDelay(pdMS_TO_TICKS(10));
    TickType_t xLastWakeTime = xTaskGetTickCount();
    char row0[17], row1[17];
    uint8_t holdCount = 0;
    uint8_t page = 0;

    for (;;) {
        float   light = AppState_GetLight();
        float   sp    = AppState_GetSetPoint();
        uint8_t pwm   = AppState_GetPwmOutput();
        float   kp    = AppState_GetKp();
        float   ki    = AppState_GetKi();
        float   kd    = AppState_GetKd();

        if (++holdCount >= LCD_PAGE_HOLD) {
            holdCount = 0;
            page = (page + 1) % 3;
        }

        // ===== LCD: 3 rotating pages =====
        switch (page) {
        case 0:
            snprintf(row0, sizeof(row0), "Lux %5.1f%%      ", light);
            snprintf(row1, sizeof(row1), "Goal %5.1f%%     ", sp);
            break;
        case 1:
            snprintf(row0, sizeof(row0), "Drive %3u/255  ", pwm);
            snprintf(row1, sizeof(row1), "Err  %+5.1f%%   ", sp - light);
            break;
        default:  // case 2: PID gains
            snprintf(row0, sizeof(row0), "P%4.2f I%4.2f", kp, ki);
            snprintf(row1, sizeof(row1), "D%5.3f      ", kd);
            break;
        }

        LcdDisplay_PrintAt(0, 0, row0);
        LcdDisplay_PrintAt(0, 1, row1);

        // ===== Teleplot output (printf, STDIO) =====
        // One line per variable, `>Name:value` — drag any onto a chart.
        // Output is PWM mapped to 0..100 % so it overlays the light trace.
        float outPct = (float)pwm * 100.0f / 255.0f;
        printf(">Light:%.2f\n",    light);
        printf(">SetPoint:%.2f\n", sp);
        printf(">Output:%.2f\n",   outPct);
        printf(">PWM:%u\n",        pwm);
        printf(">Err:%.2f\n",      sp - light);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(DISPLAY_TASK_PERIOD_MS));
    }
}
