#ifndef TASK_REPORTER_H
#define TASK_REPORTER_H

/* SRV - TaskReporter (Task3)
 * Recurrence: 10000ms | Offset: 10ms
 *
 * Every 10 seconds: prints a statistics report via printf() to Serial Monitor,
 * then resets all counters in Signals.
 */

void TaskReporter_Run(void);

#endif
