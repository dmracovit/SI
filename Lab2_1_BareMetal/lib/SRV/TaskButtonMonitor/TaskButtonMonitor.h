#ifndef TASK_BUTTON_MONITOR_H
#define TASK_BUTTON_MONITOR_H

/* SRV - TaskButtonMonitor (Task1)
 * Recurrence: 20ms | Offset: 0ms
 *
 * Detects button press/release edges on D2.
 * Measures press duration by counting 20ms ticks.
 * On release: signals duration + short/long classification to Task2.
 * Lights green LED (<500ms) or red LED (>=500ms) for 300ms (non-blocking).
 */

void TaskButtonMonitor_Run(void);

#endif
