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

// Some macros that make the code more readable
#define set_input(portdir,pin) portdir &= ~_BV(pin)
#define set_output(portdir,pin) portdir |= _BV(pin)

#define output_low(port,pin) port &= ~_BV(pin)
#define output_high(port,pin) port |= _BV(pin)
#define output_toggle(port,pin) port ^= _BV(pin)
#define input_value(port,pin) (port & _BV(pin))

#define bit_set(val, bit) output_high(val, bit)
#define bit_clear(val, bit) output_low(val, bit)
#define bit_toggle(val, bit) output_toggle(val, bit)
#define bitIsSet(val, bit) input_value(val, bit)

#define shots_on_press(val) (val & 0xF)
#define shots_on_release(val) (val & 0xF0) >> 4

#define bps_to_cycle_time(bps) 1000 / bps;
