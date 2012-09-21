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

#include "main.h"
#include "common.h"
#include "pins.h"
#include "timers.h"
#include "adc.h"
#include "settings.h"

#include "IntervalLapse.h"
#include "PinState.h"
#include "BreechEyesTask.h"

#include "GameTimer.h"

///
/// Program Specific defines - for readability
#define bps_to_cycle_time(bps) 1000 / bps;


/// Cycle Values
/// Values specific to a marker cycle
volatile CycleValues g_CycleValues = 
{
    0,                  // Flags using in marker cycle (CycleFlags)
    0,                  // Shots to fire in 'cycle' (Burst)
};

// Store in EEMEM later
///
/// Marker Settings
/// Things specific to the marker
volatile MarkerSettings g_Settings = 
{
    10,                     // Trigger Debounce
    0,                      // Current Profile
    0,                      // Shots since Service?
    { 40, 60, 550, 240 },   // Sear on, C On, C Delay, C Off
    { 10, 100, 1000 }       // Eye Detect Time, Eye Ball Reflect, Eye Timeout
};


///
/// Marker Profiles
/// Customisable by the user
MarkerProfile g_Profiles[] = 
{
    { "Semi Auto\0",    0x1, 0x0, flag_set(AT_Semi), flag_set(TA_FireOnPress) },
    { "  Pump   \0",    0x1, 0x0, flag_set(AT_Pump), flag_set(TA_FireOnPress) },
#if defined AUTO_ALLOWED
    { "Full Auto\0",    0x0, 0x0, flag_set(AT_Auto), flag_set(TA_FireOnPress) },
    { "  Burst  \0",    0x3, 0x0, flag_set(AT_Semi), flag_set(TA_FireOnPress) },
    { "Reactive \0",    0x1, 0x3, flag_set(AT_Semi), flag_set(TA_FireOnPress) | flag_set(TA_FireOnRelease) },
#endif
};

MarkerProfile* g_CurrentProfile = &g_Profiles[g_Settings.currentProfile];
unsigned char g_NumProfiles = sizeof g_Profiles/sizeof(MarkerProfile);

///
/// Used to blink an LED in the loop - to make sure the program is running
#if defined KEEP_ALIVE_ACTIVE
#define KEEP_ALIVE_PIN 5        // Pin 13
#define TRIGGER_PRESSED_PIN 4   // Pin 12
#define KEEP_ALIVE_PORT PORTB
#define KEEP_ALIVE_PORT_REG DDRB
#define KEEP_ALIVE_PULSE 1      // Because we put this in the second tick function

void keepAliveToggle()
{
    // Toggle Kepp Alive LED
    output_toggle(KEEP_ALIVE_PORT, KEEP_ALIVE_PIN);
}

IntervalLapse keepAliveTask(keepAliveToggle);
#endif

///
/// Other Setting Stuff!
///

uint8_t g_triggerPullCount;
uint8_t g_ballShotCount;

///
/// End Other Setting Stuff!
///

static void triggerToggle();
static void searDrop();
static void pneumaticsCocking();
static void pneumaticsCocked();
static void cycleComplete();
static void onSecondTick();
static void startCycle();
static void fireMarker();

PinChange triggerChangeTask(triggerToggle, &CYCLE_PIN_REG, TRIGGER_PIN);

IntervalLapse searOnTask(searDrop); 
IntervalLapse pneuDelayTask(pneumaticsCocking);
IntervalLapse pneuOnTask(pneumaticsCocked);
IntervalLapse pneuOffTask(cycleComplete);
BreechEyesTask eyeCycleTask(pneumaticsCocked);

IntervalLapse secondTickTask(onSecondTick); // in increments of 0.1ms

///
/// Game Timer
#if defined GAME_TIMER
GameTimer g_GameTimer;
#endif

#if defined SERIAL_DEBUG
unsigned char lastEyeState = ES_Empty_Seen;
#endif

#if !defined USE_ARDUINO
///
///
///
int main(void)
{	
	initMarker();
    
	for (;;) {
		loopMarker();
	}
        
	return 0;
}
#endif

///
///
///
void initMarker() 
{
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

    // stop interrupts
    cli();

    // Init timers
    timer_init();
    adc_init();

    // Setup the tasks
    secondTickTask.SetIntervalTime(10000, true);

    triggerChangeTask.SetDebounce(10);

    searOnTask.SetIntervalTime(g_Settings.timings.searOn);
    pneuDelayTask.SetIntervalTime(g_Settings.timings.pneuDel);
    pneuOnTask.SetIntervalTime(g_Settings.timings.pneuOn);
    pneuOffTask.SetIntervalTime(g_Settings.timings.pneuOff);

    eyeCycleTask.SetTaskValues(g_Settings.eyeSettings.eyeTimeout, g_Settings.eyeSettings.detectionTime, g_Settings.eyeSettings.eyeBall);

    // Kick off the taks
    triggerChangeTask.Reset();
    secondTickTask.Reset();
    
#if defined KEEP_ALIVE_ACTIVE
    keepAliveTask.SetIntervalTime( KEEP_ALIVE_PULSE, true);
    keepAliveTask.Reset();
#endif

    // start interrupts
    sei();
}

void loopMarker()
{  
#if defined SERIAL_DEBUG
    //if(lastEyeState != g_CycleValues.eyesState)
    {
    //    lastEyeState = g_CycleValues.eyesState;
    //    Serial.print("Eye State: ");
    //    Serial.println(g_CycleValues.eyesState, HEX);
    }
#endif
}

static void searDrop()
{
    output_low(CYCLE_PORT, SEAR_PIN);
}

static void pneumaticsCocking()
{
    output_high(CYCLE_PORT, PNEU_PIN);

    if(is_bit_set(g_CycleValues.flags, CF_Use_Eyes))
        eyeCycleTask.Reset();
    else
        pneuOnTask.Reset();
}

static void pneumaticsCocked()
{
    output_low(CYCLE_PORT, PNEU_PIN);
    pneuOffTask.Reset();
}

static void cycleComplete()
{
    if(g_CycleValues.shotsToGo > 0 || (is_bit_set(g_CurrentProfile->actionType, AT_Auto) && is_bit_set(g_CycleValues.flags, CF_Trigger_Pressed)))
    {
        // Fire another shot
        startCycle();
    }
    else
    {
        // Ready for next shot
        bit_clear(g_CycleValues.flags, CF_Marker_Firing);
    }
}

static void triggerToggle()
{
#if defined KEEP_ALIVE_ACTIVE
    // Toggle Trigger LED
    output_toggle(KEEP_ALIVE_PORT, TRIGGER_PRESSED_PIN);
#endif
    
    // Toggle the trigger pressed flag
    bit_toggle(g_CycleValues.flags, CF_Trigger_Pressed);

    // Do we want to fire?
    if (is_bit_set(g_CycleValues.flags, CF_Trigger_Pressed) && is_bit_set(g_CurrentProfile->triggerAction, TA_FireOnPress))
    {
        g_triggerPullCount++;
        fireMarker();
    }
    else if(!is_bit_set(g_CycleValues.flags, CF_Trigger_Pressed) && is_bit_set(g_CurrentProfile->triggerAction, TA_FireOnRelease))
    {
        fireMarker();
    }
}

static void onSecondTick()
{
#if defined SERIAL_DEBUG
    Serial.println("Pull Count(/s): " + g_triggerPullCount);
    Serial.println("Current BPS: " + g_ballShotCount);
#endif
    g_triggerPullCount = 0;
    g_ballShotCount = 0;

#if defined GAME_TIMER
    g_GameTimer.SubtractSecond();
#endif
    
#if defined KEEP_ALIVE_ACTIVE
    keepAliveTask.Update();
#endif
}

// actually fire the marker
static void startCycle()
{
    // Shot fired
    if(g_CycleValues.shotsToGo)
      g_CycleValues.shotsToGo--;

    // Increment shots fired
    g_Settings.shotsSinceLastReset++;
    g_ballShotCount++;

    // Start Pneumatics task
    pneuDelayTask.Reset();

    if(!is_bit_set(g_CycleValues.flags, CF_Training_Mode))
    {
        // Set Sear High (Release hammer)
        output_high(CYCLE_PORT, SEAR_PIN);
        // Restart sear task
        searOnTask.Reset();
    }
}

static inline void fireMarker()
{
    // Check we are ready to fire
    if(is_bit_set(g_CycleValues.flags, CF_Marker_Firing))
        return;

    bit_set(g_CycleValues.flags, CF_Marker_Firing);

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
/// Timer Tick
void onTimerTick()
{

    triggerChangeTask.Update();
    secondTickTask.Update();
    searOnTask.Update();
    pneuDelayTask.Update();
    pneuOnTask.Update();
    pneuOffTask.Update();
    eyeCycleTask.Update();

    /*if(g_CycleValues.markerState != CS_Ready_To_Fire)
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
    }*/
}

///
/// ADC Conversion Complete
void onADCReadComplete()
{
    // Read the value ADC for a value between 0-255
    eyeCycleTask.SetCurrentEye(ADCH);
}