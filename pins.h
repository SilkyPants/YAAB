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

#pragma once

#include "common.h"

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
#define CYCLE_PORT PORTA
#define CYCLE_PORT_REG DDRA
#define CYCLE_PIN_REG PINA

#define EYE_PORT PORTA
#define EYE_PORT_REG DDRA
#define TIMER_16_BIT
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#define CYCLE_PORT PORTB
#define CYCLE_PORT_REG DDRB
#define CYCLE_PIN_REG PINB

#define EYE_PORT PORTB
#define EYE_PORT_REG DDRB
#else
#define CYCLE_PORT PORTD
#define CYCLE_PORT_REG DDRD
#define CYCLE_PIN_REG PIND

#define EYE_PORT PORTC
#define EYE_PORT_REG DDRC

#define INPUT_PORT PORTB
#define INPUT_PORT_REG DDRB
#define INPUT_PIN_REG PINB
#define TIMER_16_BIT
#endif

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
// On Port B (0, 1, 2)
#define TRIGGER_PIN 0
#define PNEU_PIN 1
#define SEAR_PIN 2

// On Port B (3, 4)
#define EYE_PIN 3
#define IRED_PIN 4
#else
// On Port D (2, 6, 7)
#define TRIGGER_PIN 2
#define PNEU_PIN 6
#define SEAR_PIN 7

// On Port C (0, 2)
#define EYE_PIN 0
#define IRED_PIN 2

// On Port B (0-2)
#define UP_BUTTON_PIN 0
#define OK_BUTTON_PIN 1
#define DN_BUTTON_PIN 2

#if !defined TIMER_16_BIT
#define TIMSK TIMSK0
#endif
#endif
