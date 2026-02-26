#include "TaskReporter.h"
#include "Signals.h"
#include <stdio.h>

void TaskReporter_Run(void)
{
    uint16_t total    = Signals_GetTotalPresses();
    uint16_t shortCnt = Signals_GetShortPresses();
    uint16_t longCnt  = Signals_GetLongPresses();
    uint32_t shortDur = Signals_GetTotalShortDuration();
    uint32_t longDur  = Signals_GetTotalLongDuration();

    uint32_t avgDur = (total > 0) ? (shortDur + longDur) / total : 0;

    printf("==================\n");
    printf("=== 10s Report ===\n");
    printf("Total presses:   %u\n",   total);
    printf("Short (<500ms):  %u\n",   shortCnt);
    printf("Long  (>=500ms): %u\n",   longCnt);
    printf("Avg duration:    %lu ms\n", avgDur);
    printf("==================\n");

    /* Reset all statistics for the next interval */
    Signals_SetTotalPresses(0);
    Signals_SetShortPresses(0);
    Signals_SetLongPresses(0);
    Signals_SetTotalShortDuration(0);
    Signals_SetTotalLongDuration(0);
}
