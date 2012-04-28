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

#include "common.h"
#include "pins.h"
#include "timers.h"
#include "adc.h"
#include "interrupts.h"
#include "settings.h"
#include "IntervalLapse.h"
#include "PinState.h"

///
/// Program Specific defines - for readability
#define bps_to_cycle_time(bps) 1000 / bps;


/// Cycle Values
/// Values specific to a marker cycle
volatile CycleValues g_CycleValues = 
{
    0,                  // Flags using in marker cycle (CycleFlags)
    0,                  // Shots to fire in 'cycle' (Burst)
    CS_Ready_To_Fire,   // Marker current state
    0,                  // Cycle counter in 0.1ms increments (max 1.6383 seconds)
    0                   // Current value read from the eyes
};

// Store in EEMEM later
///
/// Marker Settings
/// Things specific to the marker
volatile MarkerSettings g_Settings = 
{
    100,                    // Trigger Debounce
    4,                      // Current Profile
    0,                      // Shots since Service?
    { 40, 60, 550, 240 },   // Sear on, C On, C Delay, C Off
    { 10, 100, 1000 }       // Eye Detect Time, Eye Ball Reflect, Eye Timeout
};


///
/// Marker Profiles
/// Customisable by the user
MarkerProfile g_Profiles[] = 
{
    { "Semi",  0x1, 0x0, AT_Semi, flag_set(TA_FireOnPress) },
    { "Pump",  0x1, 0x0, AT_Pump, flag_set(TA_FireOnPress) },
    { "Auto",  0x1, 0x0, AT_Auto, flag_set(TA_FireOnPress) },
    { "Burst", 0x3, 0x0, AT_Semi, flag_set(TA_FireOnPress) },
    { "React", 0x1, 0x3, AT_Semi, flag_set(TA_FireOnPress) | flag_set(TA_FireOnRelease) },
};

MarkerProfile* g_CurrentProfile = &g_Profiles[4];
unsigned char g_NumProfiles = sizeof g_Profiles/sizeof(MarkerProfile);

///
/// Used to blink an LED in the loop - to make sure the program is running
#define KEEP_ALIVE_ACTIVE

#if defined KEEP_ALIVE_ACTIVE
#define KEEP_ALIVE_PIN 5        // Pin 13
#define TRIGGER_PRESSED_PIN 4   // Pin 12
#define KEEP_ALIVE_PORT PORTB
#define KEEP_ALIVE_PORT_REG DDRB
#define KEEP_ALIVE_PULSE 1000
unsigned long lastKeepAlivePulse = 0;

void keepAliveToggle()
{
    // Toggle Kepp Alive LED
    output_toggle(KEEP_ALIVE_PORT, KEEP_ALIVE_PIN);
}

void triggerToggle()
{
    // Toggle Trigger LED
    output_toggle(KEEP_ALIVE_PORT, TRIGGER_PRESSED_PIN);
    
    onExternalChange();
}

IntervalLapse keepAliveTask(keepAliveToggle, KEEP_ALIVE_PULSE, true);
PinChange triggerChangeTask(triggerToggle, &PIND, TRIGGER_PIN, 1);
#endif

///
/// Enable serial output
#define SERIAL_DEBUG

#if defined SERIAL_DEBUG
unsigned char lastEyeState = ES_Empty_Seen;
#endif

///
///
///
void setup() 
{
#if defined SERIAL_DEBUG
    Serial.begin(9600);
/*
Number of profiles: 5

Size of EyeSettings: 5

Size of MarkerTiming: 6

Size of MarkerSettings: 17

Size of FiringValues: 7

Size of MarkerProfile: 9
*/
    Serial.print("Number of profiles: ");
    Serial.println(g_NumProfiles);
        
    Serial.print("Size of EyeSettings: ");
    Serial.println(sizeof(EyeSettings));
        
    Serial.print("Size of MarkerTiming: ");
    Serial.println(sizeof(MarkerTiming));
        
    Serial.print("Size of MarkerSettings: ");
    Serial.println(sizeof(MarkerSettings));
        
    Serial.print("Size of CycleValues: ");
    Serial.println(sizeof(CycleValues));
        
    Serial.print("Size of MarkerProfile: ");
    Serial.println(sizeof(MarkerProfile));
#endif

    // Setup pin direction
    set_input(CYCLE_PORT_REG, TRIGGER_PIN);
    set_output(CYCLE_PORT_REG, SEAR_PIN);
    set_output(CYCLE_PORT_REG, PNEU_PIN);

    set_output(EYE_PORT_REG, EYE_PIN);
    set_input(EYE_PORT_REG, IRED_PIN);

#if defined KEEP_ALIVE_ACTIVE
    // Set pins for Keep alive LED
    // Just to prove the loop is ticking over
    set_output(KEEP_ALIVE_PORT_REG, KEEP_ALIVE_PIN);
    set_output(KEEP_ALIVE_PORT_REG, TRIGGER_PRESSED_PIN);
#endif

    /*
    set_input(INPUT_PORT_REG, UP_BUTTON_PIN);
    set_input(INPUT_PORT_REG, OK_BUTTON_PIN);
    set_input(INPUT_PORT_REG, DN_BUTTON_PIN);
    */

    // Enable internal pullups
    output_high(PORTD, TRIGGER_PIN);
    
    /*
    output_high(INPUT_PORT, UP_BUTTON_PIN);
    output_high(INPUT_PORT, OK_BUTTON_PIN);
    output_high(INPUT_PORT, DN_BUTTON_PIN);
    */

    cli();
    timer_init();
    adc_init();
    //trigger_init();
        
    // read initial trigger state
//    if(input_value(CYCLE_PORT, TRIGGER_PIN) == LOW)
//        bit_set(g_CycleValues.flags, CF_Trigger_Pressed);
//    else
//        bit_clear(g_CycleValues.flags, CF_Trigger_Pressed);

    sei();

    
#if defined SERIAL_DEBUG
    Serial.println("Setup complete");
    Serial.println(g_CurrentProfile->profileName);
#endif
}

void loop()
{
//    if(input_value(CYCLE_PORT, TRIGGER_PIN) == LOW)
//    {
//        onExternalChange();
//#if defined KEEP_ALIVE_ACTIVE
//        output_toggle(KEEP_ALIVE_PORT, TRIGGER_PRESSED_PIN);
//#endif
//    }
  
#if defined SERIAL_DEBUG

    //if(lastEyeState != g_CycleValues.eyesState)
    {
    //    lastEyeState = g_CycleValues.eyesState;
    //    Serial.print("Eye State: ");
    //    Serial.println(input_value(CYCLE_PORT, TRIGGER_PIN), HEX);
    }
#endif

#if defined KEEP_ALIVE_ACTIVE
    unsigned long mil = millis();
    int delta = mil - lastKeepAlivePulse;
    keepAliveTask.Update(delta);
    triggerChangeTask.Update(delta);
    lastKeepAlivePulse = mil;
#endif
}

// Change marker state
inline void changeState(unsigned char newState)
{
    g_CycleValues.markerState = (CycleStates)newState;

    g_CycleValues.cycleCount = 0;
}

// actually fire the marker
inline void startCycle()
{
    //startTimer();

    // Shot fired
    if(!is_bit_set(g_CurrentProfile->actionType, AT_Auto))
        g_CycleValues.shotsToGo--;

    // Increment shots fired
    g_Settings.shotsSinceLastReset++;

    // Set Sear High (Release hammer)
    output_high(CYCLE_PORT, SEAR_PIN);

    // We are now firing
    changeState(CS_Sear_Firing);
}

inline void fireMarker()
{
    // stop counting for debounce
    //bit_clear(g_CycleValues.flags, CF_Debounce_Charge);
    
    // Determine how many shots to fire this 'cycle'
    if(is_bit_set(g_CycleValues.flags, CF_Trigger_Pressed))
        g_CycleValues.shotsToGo = g_CurrentProfile->shotsToFirePress;
    else
        g_CycleValues.shotsToGo = g_CurrentProfile->shotsToFireRelease;

    startCycle();
}

///
/// Interrupts
///

///
/// External Interrupt 0 Changed
inline void onExternalChange()
{
    // Toggle the trigger pressed flag
    bit_toggle(g_CycleValues.flags, CF_Trigger_Pressed);

    // Clear counter
    g_CycleValues.cycleCount = 0;
    
      Serial.print("Flags: ");
      Serial.print(g_CycleValues.flags, BIN);
      Serial.print(" Action: ");
      Serial.print(g_CurrentProfile->triggerAction, BIN);

    // Do we want to check for debounce?
    bool triggerPressed = is_bit_set(g_CycleValues.flags, CF_Trigger_Pressed);
    
      Serial.print(" Trigger: ");
      Serial.println(triggerPressed, BIN);

    if ((triggerPressed && is_bit_set(g_CurrentProfile->triggerAction, TA_FireOnPress)))
    {
        //bit_set(g_CycleValues.flags, CF_Debounce_Charge);
        fireMarker();
    }
    else if(!triggerPressed && is_bit_set(g_CurrentProfile->triggerAction, TA_FireOnRelease))
    {
      Serial.println("Release Fire");
        fireMarker();
    }
    else
    {
    //    bit_clear(g_CycleValues.flags, CF_Debounce_Charge);
    }
    
    //fireMarker();
    
    Serial.println(g_CycleValues.flags, BIN);
}

///
/// Timer Tick
inline void onTimerTick()
{
    if(g_CycleValues.markerState == CS_Ready_To_Fire && is_bit_set(g_CycleValues.flags, CF_Debounce_Charge))
    {
//        // increment cycle time
//        g_CycleValues.cycleCount++;
//
//        if(g_CycleValues.cycleCount >= g_Settings.debounceTime)
//        {
//            fireMarker();
//        }
    }
    else
    {
        // increment cycle time
        g_CycleValues.cycleCount++;

        switch(g_CycleValues.markerState)
        {
        case CS_Sear_Firing:
            if(bit_is_set(CYCLE_PORT, SEAR_PIN) && g_CycleValues.cycleCount >= g_Settings.timings.searOn)
            {
                // Turn off sear
                output_low(CYCLE_PORT, SEAR_PIN);
            }

            if(g_CycleValues.cycleCount >= g_Settings.timings.pneuDel)
            {
                // TODO: Something about pump!

                // Turn on pneumatics
                output_high(CYCLE_PORT, PNEU_PIN);

                changeState(CS_Breech_Opening);
            }
            break;

        case CS_Breech_Opening:
            if(is_bit_set(g_CycleValues.flags, CF_Use_Eyes))
            {
                // TODO: INSERT EYE LOGIC HERE
                // Start read if needed
                // else compare read for empty
                // then compare read for ball seen
                // Change state once ball seen long enough

            }
            else if(g_CycleValues.cycleCount == g_Settings.timings.pneuOn)
            {
                // Turn off pneumatics
                output_low(CYCLE_PORT, PNEU_PIN);
                changeState(CS_Breech_Closing);
            }
            break;

        case CS_Breech_Closing:
            if(g_CycleValues.cycleCount >= g_Settings.timings.pneuOff)
            {   

                if(g_CycleValues.shotsToGo > 0 || (is_bit_set(g_CurrentProfile->actionType, AT_Auto) && is_bit_set(g_CycleValues.flags, CF_Trigger_Pressed)))
                {
                    // Fire another shot
                    startCycle();
                }
                else
                {
                    // Ready for next shot
                    changeState(CS_Ready_To_Fire);

                    //stopTimer();
                }
            }
            break;
        };
    }
}

///
/// ADC Conversion Complete
inline void onADCReadComplete()
{
    // Read the value ADC for a value between 0-255
    if(ADCH >= g_Settings.eyeSettings.eyeBall)
        g_CycleValues.eyesState = ES_Ball_Seen;
    else
        g_CycleValues.eyesState = ES_Empty_Seen;
}



