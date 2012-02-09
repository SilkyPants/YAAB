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

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
#define CYCLE_PORT PORTA
#else
#define CYCLE_PORT PORTD
#endif

// On Port D (2, 6, 7)
#define TRIGGER_PIN 2
#define PNEU_PIN 6
#define SEAR_PIN 7

// On Port C (0, 2)
#define EYE_PIN A2
#define IRED_PIN A0

// Don't need to declare as they are hard wired lines
//#define OLED_SCL_PIN A5
//#define OLED_SDA_PIN A4

// On Port B (0-2)
#define UP_BUTTON_PIN 8
#define OK_BUTTON_PIN 9
#define DN_BUTTON_PIN 10

/*
*****************
     NOTES!!!
*****************

Time values are in 0.1 milliseconds steps, this is to allow the use of integers for calculation
and helps keep the size down

With SLOW_THINGS_DOWN defined the units will be in 0.1 seconds instead of milliseconds

To break the units into printable values get the decimal with x % 10 and the milliseconds with x / 10
*/

#define SLOW_THINGS_DOWN
#define FASTER_ADC

#ifndef F_CPU
// define F_CPU when it's not there
// Set this to clock speed
#define F_CPU 16000000U
#endif

#if defined SLOW_THINGS_DOWN
#define TIMER_FREQ 1000
#else
#define TIMER_FREQ 10000
#endif

// This prescale will give no errors in accuracy, but won't work for 20MHz CPU
#define TIMER_PRESCALE 64
#define TIMER_OVF_VAL 65536 - F_CPU/TIMER_PRESCALE/TIMER_FREQ

// Some macros that make the code more readable
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define output_toggle(port,pin) port ^= (1<<pin)
#define input_value(port,pin) (port & (1<<pin))

// Variables to track
unsigned long shotsSinceLastReset = 0;

volatile bool isFiring = false;
volatile bool debounceCharge = false;
volatile bool triggerPressed = false;
volatile uint8_t cycleCount = 0; // Keeps track of the cycle time in 0.1ms increments
volatile uint8_t shotsToGo = 0;

struct FireValues
{
    uint8_t searOn;
    uint8_t pneuDel;
    uint8_t pneuOn;
    uint8_t pneuOff;
    uint8_t debounce;
    uint8_t shotsToFire;
    
    bool releaseFire;
    bool pressFire;

    FireValues()
    {
        searOn = 40;
        pneuDel = 60;
        pneuOn = 55 + pneuDel;
        pneuOff = 24 + pneuOn;
        debounce = 10;
        shotsToFire = -1;
        releaseFire = false;
        pressFire = true;
    }

} g_DefaultValues;


///
/// Interrupts
///
/*
///
/// Init ADC
///
// initialize adc
inline void adc_init()
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);

#if defined FASTER_ADC
    // ADC Enable and prescaler of 16 and interrupt ((1 << ADIE))
    // 16000000/16 = 1000000
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1 << ADIE);
#else
    // ADC Enable and prescaler of 128 and interrupt ((1 << ADIE))
    // 16000000/128 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1 << ADIE);
#endif
}

///
/// Analog to Digital Conversion Complete
///
ISR(ADC_vect) 
{ 
    // ADC Complete
    // Read the value from ADC for a value between 0-1023
}
*/
///
/// Init Timer
///
inline void timer_init()
{
    TCCR1A = 0;
    TCCR1B = 0;

    TCNT1 = TIMER_OVF_VAL;    // preload timer
    TCCR1B |= (1 << CS11)|(1 << CS10);    // 64 prescaler
    TIMSK1 |= (1 << TOIE1);    // enable timer
}

///
/// Timer Interrupt
///
ISR(TIMER1_OVF_vect)
{
    TCNT1 = TIMER_OVF_VAL;    // reload timer
    
      bool triggerCurrent = input_value(PIND, TRIGGER_PIN) != LOW;
      
    if(!isFiring)
    {
      
      if(triggerCurrent != triggerPressed)
      {
        triggerPressed = triggerCurrent;
        
        debounceCharge = triggerPressed ? g_DefaultValues.pressFire : g_DefaultValues.releaseFire;
      }
      
      if(debounceCharge)
      {
        // increment cycle time
        cycleCount++;
      }
        
      if(cycleCount >= g_DefaultValues.debounce)
      {
        isFiring = true;
        debounceCharge = false;
        
        if(shotsToGo == 0)
          shotsToGo = g_DefaultValues.shotsToFire;
          
        cycleCount = 0;
        
        // Set Sear High (Release hammer)
        output_high(CYCLE_PORT, SEAR_PIN);
      }
    }
    else
    {
      // increment cycle time
      cycleCount++;
  
      if(cycleCount >= g_DefaultValues.searOn && bit_is_set(CYCLE_PORT, SEAR_PIN))
      {
          // Turn off sear
          output_low(CYCLE_PORT, SEAR_PIN);
            
      }
  
      if(cycleCount == g_DefaultValues.pneuDel && !bit_is_set(CYCLE_PORT, PNEU_PIN))
      {
          // Turn on pneumatics
          output_high(CYCLE_PORT, PNEU_PIN);
      }
      //
      // INSERT EYE LOGIC HERE
      //
      else if(cycleCount == g_DefaultValues.pneuOn && bit_is_set(CYCLE_PORT, PNEU_PIN))
      {
          // Turn off pneumatics
          output_low(CYCLE_PORT, PNEU_PIN);
      }
      else if(cycleCount == g_DefaultValues.pneuOff)
      {   
            // Toggle isFiring
            isFiring = false;
            
          if(--shotsToGo > 0 && triggerCurrent)
          {
            cycleCount = g_DefaultValues.debounce;
          }
          else
          {
            // Clear cycle counter
            cycleCount = 0;
          }
      }
    }
}


void setup() 
{
    pinMode(TRIGGER_PIN, INPUT);
    pinMode(SEAR_PIN, OUTPUT);
    pinMode(PNEU_PIN, OUTPUT);

/*
    pinMode(EYE_PIN, OUTPUT);
    pinMode(IRED_PIN, INPUT);
    
    pinMode(UP_BUTTON_PIN, INPUT);
    pinMode(OK_BUTTON_PIN, INPUT);
    pinMode(DN_BUTTON_PIN, INPUT);
*/
    cli();
    timer_init();
    //adc_init();
    sei();
}

void loop()
{
}

/*
// read adc value
void adc_start_read(uint8_t ch)
{
    // select the corresponding channel 0~7
    // ANDing with '7' will always keep the value
    // of 'ch' between 0 and 7
    ch &= 0b00000111;  // AND operation with 7
    ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing

    // start single conversion
    // write '1' to ADSC
    ADCSRA |= (1<<ADSC);
}*/
