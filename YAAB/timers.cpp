/*
YAAB - Yet Another Autococker Board
Arduino based autococker board developed around the platform and ATMEL AVR 
chips

Copyright (C) 2013  Dan Silk

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
#include "marker.h"

// Are we using the 16 bit timer
#define TIMER_16_BIT

/*
Time values are in 0.1 milliseconds steps, this is to allow the use of integers for calculation
and helps keep the size down

With SLOW_THINGS_DOWN defined the units will be in 0.1 seconds instead of milliseconds

To break the units into printable values get the decimal with x % 10 and the milliseconds with x / 10
*/
//#define SLOW_THINGS_DOWN

// controls the speed of the cycle timer
#if defined SLOW_THINGS_DOWN
#define TIMER_FREQ 100
#else
#define TIMER_FREQ 10000
#endif

// This prescale will give no errors in accuracy
// MUST CHANGE PRESCALER BELOW TO MATCH!!
#define TIMER_PRESCALE 8
#define TIMER_VAL F_CPU/TIMER_PRESCALE/TIMER_FREQ

#if defined TIMER_16_BIT
#define TIMER_COMPARE_VECT TIMER1_COMPA_vect
#define TIMER_CONTROL_A TCCR1A
#define TIMER_CONTROL_B TCCR1B
#define TIMER_ENABLE_MASK TIMSK1

#define TIMER_ENABLE_BIT OCIE1A
#define TIMER_CTC_MODE_BIT WGM12
#define TIMER_COUNT TCNT1
#define TIMER_OUTPUT_COMPARE_A OCR1A

#define TIMER_PRESCALER (1 << CS11)
#else
#define TIMER_COMPARE_VECT TIMER0_COMPA_vect
#define TIMER_CONTROL_A TCCR0A
#define TIMER_CONTROL_B TCCR0B
#define TIMER_ENABLE_MASK TIMSK0

#define TIMER_ENABLE_BIT OCIE0A
#define TIMER_CTC_MODE_BIT WGM01
#define TIMER_COUNT TCNT0
#define TIMER_OUTPUT_COMPARE_A OCR0A

#define TIMER_PRESCALER (1 << CS01)
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
    TIMER_CONTROL_A = 0;                    // clear A control register (we don't need anything here)
    TIMER_CONTROL_B = TIMER_PRESCALER;      // set B control register to the prescaler value
    TCCR1B |= (1 << TIMER_CTC_MODE_BIT);    // enable CTC mode

    TIMER_COUNT = 0;                        // clear the timer
    TIMER_OUTPUT_COMPARE_A = TIMER_VAL;     // set the compare value

    // Start the timer
    startTimer();
}

void startTimer()
{
    bit_set(TIMER_ENABLE_MASK, TIMER_ENABLE_BIT);
}

void stopTimer()
{
    bit_clear(TIMER_ENABLE_MASK, TIMER_ENABLE_BIT);
}

///
/// Timer Interrupt
///
ISR(TIMER_COMPARE_VECT)
{
    onTimerTick();
}

