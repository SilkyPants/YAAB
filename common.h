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

#include "Arduino.h"

//#include <avr\interrupt.h>
//#include <avr\io.h>

///
/// Some macros that make the code more readable
#define flag_set(flag) _BV(flag)
#define flag_invert(flag) ~_BV(flag)

#define bit_set(val, bit) val |= flag_set(bit)
#define bit_clear(val, bit) val &= flag_invert(bit)
#define bit_toggle(val, bit) val ^= flag_set(bit)
#define is_bit_set(val, bit) (val & flag_set(bit))

#define high_nybble(byte) ((byte >> 4) & 0x0F)
#define low_nybble(byte) ((byte) & 0x0F)

#define set_input(portdir,pin) bit_clear(portdir,pin)
#define set_output(portdir,pin) bit_set(portdir,pin)

#define output_low(port,pin) bit_clear(port,pin)
#define output_high(port,pin) bit_set(port,pin)
#define output_toggle(port,pin) bit_toggle(port,pin)
#define input_value(port,pin) is_bit_set(port,pin)

///
/// Mark functions that should always be inline
inline void changeState(unsigned char newState) __attribute__((always_inline));
inline void startCycle() __attribute__((always_inline));
inline void fireMarker() __attribute__((always_inline));
inline void onExternalChange() __attribute__((always_inline));
inline void onADCReadComplete() __attribute__((always_inline));
inline void onTimerTick() __attribute__((always_inline));