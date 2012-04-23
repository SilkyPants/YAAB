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

#include <avr\interrupt.h>
#include <avr\io.h>
#include "common.h"

///
/// Init External Interrupt 0
///
void trigger_init()
{
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    GIMSK |= _BV(INT0);  //Enable INT0
    MCUCR |= _BV(ISC00); //Trigger on change of INT0
#else
    EIMSK |= _BV(INT0);  //Enable INT0
    EICRA |= _BV(ISC10); //Trigger on change of INT0
#endif
}

///
/// External Interrupt 0
///
ISR(SIG_INTERRUPT0)
{
    onExternalChange();
}