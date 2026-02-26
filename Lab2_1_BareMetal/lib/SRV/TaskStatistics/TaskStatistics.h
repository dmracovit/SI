#ifndef TASK_STATISTICS_H
#define TASK_STATISTICS_H

/* SRV - TaskStatistics (Task2)
 * Recurrence: 100ms | Offset: 5ms
 *
 * On each new press (detected via Signals):
 *   - updates total/short/long counters and duration sums
 *   - blinks yellow LED: 5 blinks for short, 10 for long (non-blocking FSM)
 *
 * FSM states: IDLE → BLINKING → IDLE
 * New presses are registered in ANY state (no presses missed during blinking).
 */

void TaskStatistics_Run(void);

#endif
