#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

typedef struct {
    void (*task_func)(void);
    int rec;
    int offset;
    int rec_cnt;
} Task_t;

enum {
    TASK_BUTTON_MONITOR = 0,
    TASK_STATISTICS,
    TASK_REPORTER,
    MAX_OF_TASKS
};

#define REC_BUTTON_MONITOR  20
#define REC_STATISTICS      100
#define REC_REPORTER        10000

#define OFFS_BUTTON_MONITOR 0
#define OFFS_STATISTICS     5
#define OFFS_REPORTER       10

void Scheduler_Init(void);
void Scheduler_Start(void);
void Scheduler_Loop(void);

#endif
