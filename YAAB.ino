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
#define CYCLE_PORT_REG DDRA

#define EYE_PORT PORTA
#define EYE_PORT_REG DDRA
#define TIMER_16_BIT
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#define CYCLE_PORT PORTB
#define CYCLE_PORT_REG DDRB

#define EYE_PORT PORTB
#define EYE_PORT_REG DDRB
#else
#define CYCLE_PORT PORTD
#define CYCLE_PORT_REG DDRD

#define EYE_PORT PORTC
#define EYE_PORT_REG DDRC
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
// On Port D (5, 6, 7)
#define TRIGGER_PIN 5
#define PNEU_PIN 6
#define SEAR_PIN 7

// On Port C (0, 2)
#define EYE_PIN A0
#define IRED_PIN A2

// Don't need to declare as they are hard wired lines
//#define OLED_SCL_PIN A5
//#define OLED_SDA_PIN A4

// On Port B (0-2)
#define UP_BUTTON_PIN 8
#define OK_BUTTON_PIN 9
#define DN_BUTTON_PIN 10

#if !defined TIMER_16_BIT
#define TIMSK TIMSK0
#endif
#endif

/*
*****************
NOTES!!!
*****************

Time values are in 0.1 milliseconds steps, this is to allow the use of integers for calculation
and helps keep the size down

With SLOW_THINGS_DOWN defined the units will be in 0.1 seconds instead of milliseconds

To break the units into printable values get the decimal with x % 10 and the milliseconds with x / 10
*/

//#define SLOW_THINGS_DOWN
#define FASTER_ADC
//#define SERIAL_DEBUG
//#define OVERFLOW_TIMER

#ifndef F_CPU
// define F_CPU when it's not there
// Set this to clock speed
#define F_CPU 16000000U
#endif

// controls the speed of the cycle timer
#if defined SLOW_THINGS_DOWN
#define TIMER_FREQ 1000
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

// Some macros that make the code more readable
#define output_low(port,pin) port &= ~(1<<pin)
#define output_high(port,pin) port |= (1<<pin)
#define output_toggle(port,pin) port ^= (1<<pin)
#define input_value(port,pin) (port & (1<<pin))

#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

// Variables to track
unsigned long shotsSinceLastReset = 0;

volatile bool isFiring = false;
volatile bool debounceCharge = false;
volatile bool triggerPressed = false;
volatile uint8_t cycleCount = 0; // Keeps track of the cycle time in 0.1ms increments
volatile uint8_t shotsToGo = 0;
volatile bool eyesOn = false;

struct TimingValues
{
    unsigned char searOn;
    unsigned char pneuDel;
    unsigned char pneuOn;
    unsigned char pneuOff;
    unsigned char debounce;

    TimingValues()
    {
        searOn = 40;
        pneuDel = 60;
        pneuOn = 55 + pneuDel;
        pneuOff = 24 + pneuOn;
        debounce = 10;
    }
} g_TimingValues;

struct FiringValues
{
    char shotsToFirePress;
    char shotsToFireRelease;
    bool releaseFire;
    bool pressFire;

    FiringValues()
    {
        shotsToFirePress = -1;
        shotsToFireRelease = -1;
        releaseFire = false;
        pressFire = true; 
    }

} g_FiringValues;

inline void startCycle()
{
    // now we are firing
    isFiring = true;

    // stop counting for debounce
    debounceCharge = false;

    // reset cycle counter
    cycleCount = 0;

    // Set Sear High (Release hammer)
    output_high(CYCLE_PORT, SEAR_PIN);
}

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

    bool triggerCurrent = input_value(CYCLE_PORT, TRIGGER_PIN) != LOW;

    if(!isFiring)
    {

        if(triggerCurrent != triggerPressed)
        {
            triggerPressed = triggerCurrent;

            if(triggerPressed)
                debounceCharge = g_FiringValues.pressFire;
            else
                debounceCharge = g_FiringValues.releaseFire;
        }

        if(debounceCharge)
        {
            // increment cycle time
            cycleCount++;
        }

        if(cycleCount >= g_TimingValues.debounce)
        {
            startCycle();

            if(triggerPressed)
                shotsToGo = g_FiringValues.shotsToFirePress;
            else
                shotsToGo = g_FiringValues.shotsToFireRelease;
        }
    }
    else
    {
        // increment cycle time
        cycleCount++;

        if(cycleCount >= g_TimingValues.searOn && bit_is_set(CYCLE_PORT, SEAR_PIN))
        {
            // Turn off sear
            output_low(CYCLE_PORT, SEAR_PIN);

        }

        if(cycleCount == g_TimingValues.pneuDel && !bit_is_set(CYCLE_PORT, PNEU_PIN))
        {
            // Turn on pneumatics
            output_high(CYCLE_PORT, PNEU_PIN);
        }
        else if(eyesOn)
        {
            // TODO: INSERT EYE LOGIC HERE
        }
        else if(cycleCount == g_TimingValues.pneuOn && bit_is_set(CYCLE_PORT, PNEU_PIN))
        {
            // Turn off pneumatics
            output_low(CYCLE_PORT, PNEU_PIN);
        }
        else if(cycleCount == g_TimingValues.pneuOff)
        {   
            // Toggle isFiring
            isFiring = false;

            if(shotsToGo < 0 || (--shotsToGo > 0 && triggerCurrent))
            {
                cycleCount = g_TimingValues.debounce;
            }
            else
            {
                // Clear cycle counter
                cycleCount = 0;

                // Clear shot count
                shotsToGo = 0;
            }
        }
    }
}


void setup() 
{
#if defined SERIAL_DEBUG
    Serial.begin(9600);
#endif

    set_input(CYCLE_PORT_REG, TRIGGER_PIN);
    set_output(CYCLE_PORT_REG, SEAR_PIN);
    set_output(CYCLE_PORT_REG, PNEU_PIN);

    /*
    set_output(EYE_PORT_REG, EYE_PIN);
    set_input(EYE_PORT_REG, IRED_PIN);

    set_input(UP_BUTTON_PIN);
    set_input(OK_BUTTON_PIN);
    set_input(DN_BUTTON_PIN);
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
