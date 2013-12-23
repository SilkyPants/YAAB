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

#include "adc.h"
#include "marker.h"

// ADC Completes faster
#define FASTER_ADC

///
/// Analog to Digital
///

///
/// Init ADC
///
// initialize adc
void adc_init()
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);

    // Clearing all of ADCSRB will enable free running mode
    ADCSRB = 0x0;

#if defined FASTER_ADC
    // ADC Enable and prescaler of 16 and interrupt ((1 << ADIE))
    // 16000000/16 = 1000000
    ADCSRA = (1 << ADEN)|(1 << ADPS2)|(1 << ADIE);
#else
    // ADC Enable and prescaler of 128 and interrupt ((1 << ADIE))
    // 16000000/128 = 125000
    ADCSRA = (1 << ADEN)|(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0)|(1 << ADIE);
#endif

    // ADC Auto Trigger Enable
    ADCSRA |= (1 << ADATE);
}

///
/// Start Read ADC Channel
///
void adc_start_read(uint8_t ch)
{
    // select the corresponding channel 0~7
    // ANDing with '7' will always keep the value
    // of 'ch' between 0 and 7
    ch &= 0x7;                  // AND operation with 7
    ADMUX = (ADMUX & 0xF8) | ch;  // clears the bottom 3 bits before ORing
    
    // start single conversion
    // write '1' to ADSC
    ADCSRA |= (1<<ADSC);
}

void adc_stop_all()
{
    // stop auto run
    ADCSRA &= ~(1<<ADATE);

    // stop conversion
    ADCSRA &= ~(1<<ADSC);
}


///
/// Analog to Digital Conversion Complete
///
ISR(ADC_vect) 
{ 
    onADCReadComplete();
}

