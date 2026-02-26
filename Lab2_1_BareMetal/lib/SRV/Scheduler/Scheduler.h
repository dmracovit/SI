#ifndef SCHEDULER_H
#define SCHEDULER_H

/* SRV - Scheduler
 * Non-preemptive bare-metal sequential task scheduler.
 *
 * Each task has a recurrence (period in ms) and an offset (initial delay).
 * The ISR (Timer1, 1ms tick) calls Scheduler_Loop() which decrements each
 * task's counter and executes the task when the counter reaches zero.
 *
 * Task table:
 *   TASK_BUTTON_MONITOR  rec=20ms   offset=0ms
 *   TASK_STATISTICS      rec=100ms  offset=5ms
 *   TASK_REPORTER        rec=10000ms offset=10ms
 */

#include <Arduino.h>

typedef struct {
    void (*task_func)(void); // pointer to the task function
    int rec;                  // recurrence period in ms
    int offset;               // initial delay in ms (for staggering task starts)
    int rec_cnt;              // current countdown (decremented each tick)
} Task_t;

enum {
    TASK_BUTTON_MONITOR = 0,
    TASK_STATISTICS,
    TASK_REPORTER,
    MAX_OF_TASKS
};

#define REC_BUTTON_MONITOR   20
#define REC_STATISTICS       100
#define REC_REPORTER         10000

#define OFFS_BUTTON_MONITOR  0
#define OFFS_STATISTICS      5
#define OFFS_REPORTER        10

void Scheduler_Init(void);
void Scheduler_Start(void);  // starts Timer1 and enables global interrupts
void Scheduler_Loop(void);   // called from ISR every 1ms

#endif
