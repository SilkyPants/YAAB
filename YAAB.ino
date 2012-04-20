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

//#define SERIAL_DEBUG

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

void changeState(byte newState)
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

inline void onExternalChange()
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

inline void onADCReadComplete()
{
    // Read the value ADC for a value between 0-255
    if(ADCH >= g_Settings.eyeSettings.eyeBall)
        g_FiringValues.eyesState = ES_Ball_Seen;
    else if(ADCH >= g_Settings.eyeSettings.eyeBolt)
        g_FiringValues.eyesState = ES_Bolt_Seen;
    else
        g_FiringValues.eyesState = ES_Empty_Seen;
}

inline void onTimerTick()
{
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
        bit_set(g_FiringValues.flags, FF_TriggerPressed);
    else
        bit_clear(g_FiringValues.flags, FF_TriggerPressed);

    sei();
}

void loop()
{
}



