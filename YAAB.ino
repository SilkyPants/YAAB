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
#define EYE_PIN 0
#define IRED_PIN 2

// Don't need to declare as they are hard wired lines
//#define OLED_SCL_PIN A5
//#define OLED_SDA_PIN A4

// On Port B (0-2)
#define UP_BUTTON_PIN 0
#define OK_BUTTON_PIN 1
#define DN_BUTTON_PIN 2

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

enum ProfileFlags
{
    PF_Pump,
    PF_Semi,
    PF_Auto,
    PF_FireOnPress,
    PF_FireOnRelease,
    PF_CappedROF,
    PF_Reserved1,
    PF_Reserved2
};

enum FiringFlags
{
    FF_Use_Eyes,
    FF_TriggerPressed,
    FF_DebounceCharge,
    FF_Reserved,
};

enum CycleStates
{
    CS_Ready_To_Fire,
    CS_Sear_Firing,
    CS_Breech_Opening,
    CS_Breech_Closing,
};

enum EyeStates
{
    ES_Empty_Seen,
    ES_Ball_Seen,
    ES_Bolt_Seen,
    ES_Reserved,
};

struct EyeSettings // 4 bytes
{
    byte ballDetection;     // Time before ball is detected
    byte emptyDetection;    // Time before empty is detected
    byte eyeBall;           // Value for ball seen
    byte eyeBolt;           // Value for bolt seen
};

struct MarkerTiming // 4 bytes?
{
    byte searOn : 6;
    byte pneuDel : 6;
    uint16_t pneuOn : 10;
    uint16_t pneuOff : 10;
    
    void ResetFast()
    {
        searOn = 40;
        pneuDel = 60;
        pneuOn = 550;
        pneuOff = 240;
    }

    void ResetMedium()
    {
        searOn = 40;
        pneuDel = 60;
        pneuOn = 650;
        pneuOff = 280;
    }

    void ResetSlow()
    {
        searOn = 40;
        pneuDel = 60;
        pneuOn = 750;
        pneuOff = 320;
    }
};

struct MarkerSettings // 16 bytes?
{
    MarkerTiming timings;
    EyeSettings eyeSettings;
    byte currentProfile; 
    byte debounce;
    uint16_t eyeTimeout;
    uint32_t shotsSinceLastService;
};

struct FiringValues // 3 bytes?
{
    byte flags : 2;             // Flags using in marker cycle (FiringFlags)
    byte shotsToGo : 4;         // Shots to fire in 'cycle' (Burst)
    byte markerState : 2;       // Marker current state
    byte eyesState : 2;         // Eyes current state
    uint16_t cycleCount : 14;   // Cycle counter in 0.1ms increments (max 1.6383 seconds)
};

///
/// Profiles
///

struct FiringProfile // 8 bytes
{
    char profileName[6];            // Name for the profile
    byte profileFlags : 4;          // Flags used while cycling (ProfileFlags)
    byte shotsToFirePress : 4;      // Burst fire on press (max 15)
    byte shotsToFireRelease : 4;    // Burst fire on release (max 15)
    byte reserved : 4;              // Padding
};

// Values specific to a marker cycle
volatile FiringValues g_FiringValues = 
{
    0,                  // Flags using in marker cycle (FiringFlags)
    0,                  // Shots to fire in 'cycle' (Burst)
    CS_Ready_To_Fire,   // Marker current state
    0,                  // Cycle counter in 0.1ms increments (max 1.6383 seconds)
    0                   // Current value read from the eyes
};

// Store in EEMEM later
// Marker Settings - things specific to the marker
volatile MarkerSettings g_Settings = 
{
    { 40, 20, 550, 240 },   // Sear on, C Delay, C On, C Off
    { 10, 10, 100, 70 },    // Eye Ball Detect, Eye Empty Detect, Eye Ball Reflect, Eye Bolt Reflect
    0,                      // Current Profile
    10,                     // Trigger Debounce
    1000,                   // C Timeout / Eye Timeout
    0                       // Shots since Service?
};

// User Profiles
FiringProfile g_Profiles[] = 
{
    { "Semi", _BV(PF_Semi)|_BV(PF_FireOnPress), 0x1, 0x0 },
    { "Pump", _BV(PF_Pump)|_BV(PF_FireOnPress), 0x1, 0x0 },
    //{ "Auto", _BV(PF_Auto)|_BV(PF_FireOnPress), 0x1, 0x0 },
    //{ "Burst", _BV(PF_Semi)|_BV(PF_FireOnPress), 0x3, 0x0 },
    //{ "React", _BV(PF_Semi)|_BV(PF_FireOnPress)|_BV(PF_FireOnRelease), 0x1, 0x3 },
};

FiringProfile* g_CurrentProfile = &g_Profiles[g_Settings.currentProfile];
byte g_NumProfiles = sizeof g_Profiles/sizeof(FiringProfile);

///
///
///

inline void changeState(byte newState)
{
    g_FiringValues.markerState = (CycleStates)newState;

    g_FiringValues.cycleCount = 0;
}

void startCycle()
{
    // stop counting for debounce
    bit_clear(g_CurrentProfile->profileFlags, FF_DebounceCharge);

    // Shot fired
    g_FiringValues.shotsToGo--;

    // Increment shots fired
    g_Settings.shotsSinceLastService++;

    // Set Sear High (Release hammer)
    output_high(CYCLE_PORT, SEAR_PIN);

    // We are now firing
    changeState(CS_Sear_Firing);
}

void fireMarker()
{
    // Determine how many shots to fire this 'cycle'
    if(bitIsSet(g_FiringValues.flags, FF_TriggerPressed))
        g_FiringValues.shotsToGo = shots_on_press(g_CurrentProfile->shotsToFirePress);
    else
        g_FiringValues.shotsToGo = shots_on_release(g_CurrentProfile->shotsToFireRelease);

    startCycle();
}

///
/// Interrupts
///

///
/// Analog to Digital
///

///
/// Init ADC
///
// initialize adc
inline void adc_init()
{
    adc_stop_all();

    // AREF = AVcc
    ADMUX = (1<<REFS0);

    // 8-bit resolution
    ADMUX |= (1 << ADLAR);

    // ADC Auto Trigger Enable
    ADCSRA |= (1 << ADATE);

    // Clearing all of ADCSRB will enable free running mode
    ADCSRB = 0x0;

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
    // ADC Cycle Complete
    // Read the value ADC for a value between 0-255
    if(ADCH >= g_Settings.eyeSettings.eyeBall)
        g_FiringValues.eyesState = ES_Ball_Seen;
    else if(ADCH >= g_Settings.eyeSettings.eyeBolt)
        g_FiringValues.eyesState = ES_Bolt_Seen;
    else
        g_FiringValues.eyesState = ES_Empty_Seen;
}

///
/// Trigger Interrupt
///

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
    // Toggle the trigger pressed flag
    bit_toggle(g_FiringValues.flags, FF_TriggerPressed);

    // Clear counter
    g_FiringValues.cycleCount = 0;

    // Do we want to check for debounce?
    if ((bitIsSet(g_FiringValues.flags, FF_TriggerPressed) && bitIsSet(g_CurrentProfile->profileFlags, PF_FireOnPress)) 
    || (!bitIsSet(g_FiringValues.flags, FF_TriggerPressed) && bitIsSet(g_CurrentProfile->profileFlags, PF_FireOnRelease)))
    {
        bit_set(g_FiringValues.flags, FF_DebounceCharge);
    }
    else
    {
        bit_clear(g_FiringValues.flags, FF_DebounceCharge);
    }
}

///
/// Timer (0.1 ms)
///

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

    if(g_FiringValues.markerState == CS_Ready_To_Fire)
    {
        if(bitIsSet(g_FiringValues.flags, FF_DebounceCharge))
        {
            // increment cycle time
            g_FiringValues.cycleCount++;
        }

        if(g_FiringValues.cycleCount >= g_Settings.debounce)
        {
            fireMarker();
        }
    }
    else
    {
        // increment cycle time
        g_FiringValues.cycleCount++;

        switch(g_FiringValues.markerState)
        {
        case CS_Sear_Firing:
            if(bit_is_set(CYCLE_PORT, SEAR_PIN) && g_FiringValues.cycleCount >= g_Settings.timings.searOn)
            {
                // Turn off sear
                output_low(CYCLE_PORT, SEAR_PIN);
            }

            if(g_FiringValues.cycleCount >= g_Settings.timings.pneuDel)
            {
                // TODO: Something about pump!

                // Turn on pneumatics
                output_high(CYCLE_PORT, PNEU_PIN);

                changeState(CS_Breech_Opening);
            }
            break;

        case CS_Breech_Opening:
            if(bitIsSet(g_FiringValues.flags, FF_Use_Eyes))
            {
                // TODO: INSERT EYE LOGIC HERE
                // Start read if needed
                // else compare read for empty
                // then compare read for ball seen
                // Change state once ball seen long enough

            }
            else if(g_FiringValues.cycleCount == g_Settings.timings.pneuOn)
            {
                // Turn off pneumatics
                output_low(CYCLE_PORT, PNEU_PIN);
                changeState(CS_Breech_Closing);
            }
            break;

        case CS_Breech_Closing:
            if(g_FiringValues.cycleCount == g_Settings.timings.pneuOff)
            {   

                if(g_FiringValues.shotsToGo < 0 || (bitIsSet(g_CurrentProfile->profileFlags, PF_Auto) && bitIsSet(g_FiringValues.flags, FF_TriggerPressed)))
                {
                    // Fire another shot
                    startCycle();
                }
                else
                {
                    // Ready for next shot
                    changeState(CS_Ready_To_Fire);

                    // Clear shot count
                    g_FiringValues.shotsToGo = 0;
                }
            }
            break;
        };
    }
}


void setup() 
{
#if defined SERIAL_DEBUG
    Serial.begin(9600);

    Serial.print("Number of profiles: ");
    Serial.println(g_NumProfiles);
        
    Serial.print("Size of EyeSettings: ");
    Serial.println(sizeof(EyeSettings));
        
    Serial.print("Size of MarkerTiming: ");
    Serial.println(sizeof(MarkerTiming));
        
    Serial.print("Size of MarkerSettings: ");
    Serial.println(sizeof(MarkerSettings));
        
    Serial.print("Size of FiringValues: ");
    Serial.println(sizeof(FiringValues));
        
    Serial.print("Size of FiringProfile: ");
    Serial.println(sizeof(FiringProfile));
#endif

    // Setup pin direction
    set_input(CYCLE_PORT_REG, TRIGGER_PIN);
    set_output(CYCLE_PORT_REG, SEAR_PIN);
    set_output(CYCLE_PORT_REG, PNEU_PIN);

    set_output(EYE_PORT_REG, EYE_PIN);
    set_input(EYE_PORT_REG, IRED_PIN);

    /*
    set_input(UP_BUTTON_PIN);
    set_input(OK_BUTTON_PIN);
    set_input(DN_BUTTON_PIN);
    */

    // Enable internal pullups

    cli();
    timer_init();
    adc_init();
    trigger_init();
        
    // read initial trigger state
    if(input_value(CYCLE_PORT, TRIGGER_PIN) == LOW)
        bit_set(g_FiringValues.flags, FF_TriggerPressed);
    else
        bit_clear(g_FiringValues.flags, FF_TriggerPressed);

    sei();
}

void loop()
{
}



