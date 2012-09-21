/*
YAAB - Yet Another Autococker Board
Arduino based autococker board developed around the platform and ATMEL AVR 
chips

Copyright (C) 2012  Dan Silk

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "timers.h"
#include "common.h"
#include "pins.h"
#include "main.h"

/*
Time values are in 0.1 milliseconds steps, this is to allow the use of integers for calculation
and helps keep the size down

With SLOW_THINGS_DOWN defined the units will be in 0.1 seconds instead of milliseconds

To break the units into printable values get the decimal with x % 10 and the milliseconds with x / 10
*/
//#define SLOW_THINGS_DOWN

// This allows use of the overflow timer instead of compare
//#define OVERFLOW_TIMER

// controls the speed of the cycle timer
#if defined SLOW_THINGS_DOWN
#define TIMER_FREQ 100
#else
#define TIMER_FREQ 10000
#endif

// This prescale will give no errors in accuracy, but won't work for 20MHz CPU
#if defined TIMER_16_BIT
#define TIMER_PRESCALE 64
#else
#define TIMER_PRESCALE 8
#endif

#define TIMER_VAL F_CPU/TIMER_PRESCALE/TIMER_FREQ

#if defined OVERFLOW_TIMER
#if defined TIMER_16_BIT
#define TIMER_OVF_VAL 65536 - TIMER_VAL
#else
#define TIMER_OVF_VAL 255 - TIMER_VAL
#endif
#endif

///
/// Timer Tick Function Prototype
void onTimerTick();

///
/// Timer (0.1 ms)
///

///
/// Init Timer
///
void timer_init()
{
#if defined TIMER_16_BIT
    TCCR1A = 0;
    TCCR1B = 0;

    TCCR1B |= (1 << CS11)|(1 << CS10);    // 64 prescaler

#if defined OVERFLOW_TIMER
    TCNT1 = TIMER_OVF_VAL;      // preload timer
    TIMSK1 |= (1 << TOIE1);    // enable timer
#else
    TCNT1 = 0;
    OCR1A = TIMER_VAL;
    TCCR1B |= (1 << WGM12);     // enable CTC mode
    TIMSK1 |= (1 << OCIE1A);    // enable timer
#endif
#else
    TCCR0A = 0;
    TCCR0B = 0;

    TCCR0B |= (1 << CS01);    // 8 prescaler

#if defined OVERFLOW_TIMER
    TCNT0 = TIMER_OVF_VAL;      // preload timer
    TIMSK0 |= (1 << TOIE0);     // enable timer
#else
    TCNT0 = 0;
    OCR0A = TIMER_VAL;
    TCCR0B |= (1 << WGM01);     // enable CTC mode
    TIMSK |= (1 << OCIE0A);    // enable timer
#endif
#endif
}

void startTimer()
{
    bit_set(TIMSK1, OCIE1A);
}

void stopTimer()
{
    bit_clear(TIMSK1, OCIE1A);
}

///
/// Timer Interrupt
///
#if defined TIMER_16_BIT
#if defined OVERFLOW_TIMER
ISR(TIMER1_OVF_vect)
#else
ISR(TIMER1_COMPA_vect)
#endif
#else
#if defined OVERFLOW_TIMER
ISR(TIMER0_OVF_vect)
#else
ISR(TIMER0_COMPA_vect)
#endif
#endif
{
#if defined OVERFLOW_TIMER
#if defined TIMER_16_BIT
    TCNT1 = TIMER_OVF_VAL;    // reload timer
#else
    TCNT0 = TIMER_OVF_VAL;    // reload timer
#endif
#endif

    onTimerTick();
}
