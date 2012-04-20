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
    { 40, 20, 550, 240 },   // Sear on, C Delay, C On, C Off
    { 10, 10, 100, 70 },    // Eye Ball Detect, Eye Empty Detect, Eye Ball Reflect, Eye Bolt Reflect
    0,                      // Current Profile
    10,                     // Trigger Debounce
    1000,                   // C Timeout / Eye Timeout
    0                       // Shots since Service?
};


///
/// Marker Profiles
/// Customisable by the user
MarkerProfile g_Profiles[] = 
{
    { "Semi", _BV(PF_Semi)|_BV(PF_FireOnPress), 0x1, 0x0 },
    { "Pump", _BV(PF_Pump)|_BV(PF_FireOnPress), 0x1, 0x0 },
    //{ "Auto", _BV(PF_Auto)|_BV(PF_FireOnPress), 0x1, 0x0 },
    //{ "Burst", _BV(PF_Semi)|_BV(PF_FireOnPress), 0x3, 0x0 },
    //{ "React", _BV(PF_Semi)|_BV(PF_FireOnPress)|_BV(PF_FireOnRelease), 0x1, 0x3 },
};

MarkerProfile* g_CurrentProfile = &g_Profiles[g_Settings.currentProfile];
byte g_NumProfiles = sizeof g_Profiles/sizeof(MarkerProfile);

//#define SERIAL_DEBUG

#if defined SERIAL_DEBUG
EyeStates lastEyeState = ES_Empty_Seen;
#endif

///
///
///
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

    /*
    set_input(INPUT_PORT_REG, UP_BUTTON_PIN);
    set_input(INPUT_PORT_REG, OK_BUTTON_PIN);
    set_input(INPUT_PORT_REG, DN_BUTTON_PIN);
    */

    // Enable internal pullups
    output_high(CYCLE_PORT, TRIGGER_PIN);

    /*
    output_high(INPUT_PORT, UP_BUTTON_PIN);
    output_high(INPUT_PORT, OK_BUTTON_PIN);
    output_high(INPUT_PORT, DN_BUTTON_PIN);
    */

    cli();
    timer_init();
    adc_init();
    trigger_init();
        
    // read initial trigger state
    if(input_value(CYCLE_PORT, TRIGGER_PIN) == LOW)
        bit_set(g_CycleValues.flags, CF_Trigger_Pressed);
    else
        bit_clear(g_CycleValues.flags, CF_Trigger_Pressed);

    sei();

    
#if defined SERIAL_DEBUG
    Serial.println("Setup complete");
#endif
}

void loop()
{
#if defined SERIAL_DEBUG
    if(lastEyeState != g_CycleValues.eyesState)
    {
        lastEyeState == g_CycleValues.eyesState;
        Serial.print("Eye State: ");
        Serial.println(lastEyeState, HEX);
    }
#endif
}

// Change marker state
void changeState(byte newState)
{
    g_CycleValues.markerState = (CycleStates)newState;

    g_CycleValues.cycleCount = 0;
}

// actually fire the marker
void startCycle()
{
    // stop counting for debounce
    bit_clear(g_CurrentProfile->profileFlags, CF_Debounce_Charge);

    // Shot fired
    g_CycleValues.shotsToGo--;

    // Increment shots fired
    g_Settings.shotsSinceLastReset++;

    // Set Sear High (Release hammer)
    output_high(CYCLE_PORT, SEAR_PIN);

    // We are now firing
    changeState(CS_Sear_Firing);
}

void fireMarker()
{
    // Determine how many shots to fire this 'cycle'
    if(bitIsSet(g_CycleValues.flags, CF_Trigger_Pressed))
        g_CycleValues.shotsToGo = shots_on_press(g_CurrentProfile->shotsToFirePress);
    else
        g_CycleValues.shotsToGo = shots_on_release(g_CurrentProfile->shotsToFireRelease);

    startCycle();
}

///
/// Interrupts
///

inline void onExternalChange()
{
    // Toggle the trigger pressed flag
    bit_toggle(g_CycleValues.flags, CF_Trigger_Pressed);

    // Clear counter
    g_CycleValues.cycleCount = 0;

    // Do we want to check for debounce?
    if ((bitIsSet(g_CycleValues.flags, CF_Trigger_Pressed) && bitIsSet(g_CurrentProfile->profileFlags, PF_FireOnPress)) 
    || (!bitIsSet(g_CycleValues.flags, CF_Trigger_Pressed) && bitIsSet(g_CurrentProfile->profileFlags, PF_FireOnRelease)))
    {
        bit_set(g_CycleValues.flags, CF_Debounce_Charge);
    }
    else
    {
        bit_clear(g_CycleValues.flags, CF_Debounce_Charge);
    }
}

inline void onADCReadComplete()
{
    // Read the value ADC for a value between 0-255
    if(ADCH >= g_Settings.eyeSettings.eyeBall)
        g_CycleValues.eyesState = ES_Ball_Seen;
    else if(ADCH >= g_Settings.eyeSettings.eyeBolt)
        g_CycleValues.eyesState = ES_Bolt_Seen;
    else
        g_CycleValues.eyesState = ES_Empty_Seen;
}

inline void onTimerTick()
{
    if(g_CycleValues.markerState == CS_Ready_To_Fire)
    {
        if(bitIsSet(g_CycleValues.flags, CF_Debounce_Charge))
        {
            // increment cycle time
            g_CycleValues.cycleCount++;
        }

        if(g_CycleValues.cycleCount >= g_Settings.debounce)
        {
            fireMarker();
        }
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
            if(bitIsSet(g_CycleValues.flags, CF_Use_Eyes))
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
            if(g_CycleValues.cycleCount == g_Settings.timings.pneuOff)
            {   

                if(g_CycleValues.shotsToGo < 0 || (bitIsSet(g_CurrentProfile->profileFlags, PF_Auto) && bitIsSet(g_CycleValues.flags, CF_Trigger_Pressed)))
                {
                    // Fire another shot
                    startCycle();
                }
                else
                {
                    // Ready for next shot
                    changeState(CS_Ready_To_Fire);

                    // Clear shot count
                    g_CycleValues.shotsToGo = 0;
                }
            }
            break;
        };
    }
}



