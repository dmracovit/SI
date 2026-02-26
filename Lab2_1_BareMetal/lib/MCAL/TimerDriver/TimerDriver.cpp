#include "TimerDriver.h"
#include "Scheduler.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void TimerDriver_Init(void)
{
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    OCR1A  = 249;
    TIMSK1 = (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)
{
    sei();
    Scheduler_Loop();
}
