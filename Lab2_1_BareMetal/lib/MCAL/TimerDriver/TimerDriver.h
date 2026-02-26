#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

/* MCAL - TimerDriver
 * Configures Timer1 in CTC mode for a 1ms tick.
 * The ISR calls Scheduler_Loop() on every tick.
 *
 * Timer1 CTC config:
 *   prescaler = 64, OCR1A = 249
 *   16MHz / 64 / 250 = 1000 Hz → 1ms period
 */

#include <Arduino.h>

void TimerDriver_Init(void);

#endif
