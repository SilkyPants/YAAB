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

#pragma once

#include "common.h"

#if defined(__AVR_ATtiny84__)

#define CYCLE_PORT PORTA
#define CYCLE_PORT_REG DDRA
#define CYCLE_PIN_REG PINA

#define EYE_PORT PORTA
#define EYE_PORT_REG DDRA

#elif defined(__AVR_ATtiny85__)

#if F_CPU > 8000000UL
    #error Cannot use an ATTiny*5 chip with a clock speed greater than 8Mhz. Not enough pins!
#endif

#define CYCLE_PORT PORTB
#define CYCLE_PORT_REG DDRB
#define CYCLE_PIN_REG PINB

#define EYE_PORT PORTB
#define EYE_PORT_REG DDRB

#define TIMSK0 TIMSK // As the ATTiny*5 does not refer to timer 0 as such

#elif defined( __AVR_ATmega328__ ) || defined( __AVR_ATmega328P__ )
#define CYCLE_PORT PORTD
#define CYCLE_PORT_REG DDRD
#define CYCLE_PIN_REG PIND

#define EYE_PORT PORTC
#define EYE_PORT_REG DDRC

#define INPUT_PORT PORTB
#define INPUT_PORT_REG DDRB
#define INPUT_PIN_REG PINB

#else

#error Unsupported chip! Please edit pins.h to support your chosen AVR chip.

#endif

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
// On Port B (0, 1, 2)
#define TRIGGER_PIN 0
#define PNEU_PIN 1
#define SEAR_PIN 2

// On Port B (3, 4)
#define EYE_PIN 3
#define IRED_PIN 4
#elif defined( __AVR_ATmega328__ ) || defined( __AVR_ATmega328P__ )
// On Port D (5, 6, 7)
#define TRIGGER_PIN 5	        // Arduino Pin 5
#define PNEU_PIN 6		// Arduino Pin 6
#define SEAR_PIN 7		// Arduino Pin 7

// On Port C (0, 2)
#define EYE_PIN 0		// Arduino Pin A0
#define IRED_PIN 1		// Arduino Pin A1

// On Port B (0-2)
#define UP_BUTTON_PIN 0	// Arduino Pin 8
#define OK_BUTTON_PIN 1	// Arduino Pin 9
#define DN_BUTTON_PIN 2	// Arduino Pin 10
#endif

