#include "Scheduler.h"
#include "TimerDriver.h"
#include "TaskButtonMonitor.h"
#include "TaskStatistics.h"
#include "TaskReporter.h"
#include <avr/interrupt.h>

static Task_t tasks[MAX_OF_TASKS];

static void Scheduler_TaskInit(Task_t *task, void (*func)(void), int rec, int offset)
{
    task->task_func = func;
    task->rec = rec;
    task->offset = offset;
    task->rec_cnt = offset;
}

void Scheduler_Init(void)
{
    Scheduler_TaskInit(&tasks[TASK_BUTTON_MONITOR], TaskButtonMonitor_Run,
                       REC_BUTTON_MONITOR, OFFS_BUTTON_MONITOR);
    Scheduler_TaskInit(&tasks[TASK_STATISTICS], TaskStatistics_Run,
                       REC_STATISTICS, OFFS_STATISTICS);
    Scheduler_TaskInit(&tasks[TASK_REPORTER], TaskReporter_Run,
                       REC_REPORTER, OFFS_REPORTER);
}

void Scheduler_Start(void)
{
    TimerDriver_Init();
    sei();
}

void Scheduler_Loop(void)
{
    for (int i = 0; i < MAX_OF_TASKS; i++)
    {
        if (--tasks[i].rec_cnt <= 0)
        {
            tasks[i].rec_cnt = tasks[i].rec;
            tasks[i].task_func();
        }
    }
}
