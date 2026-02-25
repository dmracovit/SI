#include "TimerDriver.h"
#include "Scheduler.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void TimerDriver_Init(void)
{
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // CTC, prescaler 64
    OCR1A = 249;                                          // 16MHz/64/250 = 1ms
    TIMSK1 = (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
    sei(); // re-enable interrupts (needed for Serial/printf in Task3)
    Scheduler_Loop();
}
