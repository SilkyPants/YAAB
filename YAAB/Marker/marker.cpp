/*
YAAB - Yet Another Autococker Board
Arduino based autococker board developed around the platform and ATMEL AVR
chips

Copyright (C) 2015  Dan Silk

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

#include <avr/pgmspace.h>

#include "marker.h"

#include "../AVR/pins.h"
#include "../AVR/timers.h"
#include "../AVR/adc.h"
#include "../AVR/i2c.h"

#include "settings.h"

#include "Tasks/IntervalLapse.h"
#include "Tasks/PinState.h"
#include "Tasks/BreechEyesTask.h"

#include "Timer.h"

///
/// Global marker object
Marker g_Marker;

/// Cycle Values
/// Values specific to a marker cycle
volatile CycleValues g_CycleValues =
{
    0,                  // Flags using in marker cycle (CycleFlags)
    0,                  // Shots to fire in 'cycle' (Burst)
};

///
/// Store in EEMEM later
///

///
/// Marker Settings
/// Things specific to the marker
MarkerSettings g_Settings =
{
    10,                     // Trigger Debounce
    GSM_Graphic,              // Game Screen Mode
    0,                      // Graphic Index
    0,                      // Current Profile
    0,                      // Tounament Lock
    0,                      // Shots since Service?
    { 40, 60, 550, 240 },   // SON, PDEL, PON, POFF
    { 10, 100, 1000 }       // Eye Detect Time, Eye Ball Reflect, Eye Timeout
};


///
/// Marker Profiles
/// Customisable by the user

PROGMEM const MarkerProfile g_Modes[] =
{
    { 0,    0x1, 0x0, flag_set(AT_Semi), flag_set(TA_FireOnPress) },
    { 1,    0x1, 0x0, flag_set(AT_Pump), flag_set(TA_FireOnPress) },
#if defined AUTO_ALLOWED
    { 2,    0x0, 0x0, flag_set(AT_Auto), flag_set(TA_FireOnPress) },
    { 3,    0x3, 0x0, flag_set(AT_Semi), flag_set(TA_FireOnPress) },
    { 4,    0x1, 0x3, flag_set(AT_Semi), flag_set(TA_FireOnPress) | flag_set(TA_FireOnRelease) },
#endif
};

///
/// End EEPROM stuff
///

MarkerProfile g_CurrentMode;
//unsigned char g_NumProfiles = sizeof g_Modes/sizeof(MarkerProfile);

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
    // Toggle Keep Alive LED
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

///
/// Forward declare the functions
static void triggerToggle();
static void searDrop();
static void pneumaticsCocking();
static void pneumaticsCocked();
static void cycleComplete();
static void onSecondTick();
static void startCycle();
static void fireMarker();

void onGameLapsed();
void onAlarmLapsed();

PinChange triggerChangeTask(triggerToggle, &CYCLE_PIN_REG, TRIGGER_PIN);

IntervalLapse searOnTask(searDrop);
IntervalLapse pneuDelayTask(pneumaticsCocking);
IntervalLapse pneuOnTask(pneumaticsCocked);
IntervalLapse pneuOffTask(cycleComplete);
BreechEyesTask eyeCycleTask(pneumaticsCocked);

IntervalLapse secondTickTask(onSecondTick); // in increments of 0.1ms

///
/// Game Timer
Timer g_GameTimer(onGameLapsed, onAlarmLapsed);

/// -------------------------------------------------------------------------------
/// Firing Cycle
/// -------------------------------------------------------------------------------

static void triggerToggle()
{
#if defined KEEP_ALIVE_ACTIVE
    // Toggle Trigger LED
    output_toggle(KEEP_ALIVE_PORT, TRIGGER_PRESSED_PIN);
#endif

    // Toggle the trigger pressed flag
    bit_toggle(g_CycleValues.flags, CF_Trigger_Pressed);

    // Do we want to fire?
    if (is_bit_set(g_CycleValues.flags, CF_Trigger_Pressed) && is_bit_set(g_CurrentMode.triggerAction, TA_FireOnPress))
    {
        g_triggerPullCount++;
        fireMarker();
    }
    else if(!is_bit_set(g_CycleValues.flags, CF_Trigger_Pressed) && is_bit_set(g_CurrentMode.triggerAction, TA_FireOnRelease))
    {
        fireMarker();
    }
}

static void fireMarker()
{
    // Check we can fire
    if(is_bit_set(g_CycleValues.flags, CF_Marker_Firing))
        return;

    // Mark that we are firing
    bit_set(g_CycleValues.flags, CF_Marker_Firing);

    // Determine how many shots to fire this 'cycle'
    if(is_bit_set(g_CycleValues.flags, CF_Trigger_Pressed))
        g_CycleValues.shotsToGo = g_CurrentMode.shotsToFirePress;
    else
        g_CycleValues.shotsToGo = g_CurrentMode.shotsToFireRelease;

    startCycle();
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
    if(!is_bit_set(g_CurrentMode.actionType, AT_Pump))
    {
        pneuDelayTask.Reset();
    }
    else
    {
        // TODO: Start eye task to look for complete cycle
    }

    if(!is_bit_set(g_CycleValues.flags, CF_Training_Mode))
    {
        // Set Sear High (Release hammer)
        output_high(CYCLE_PORT, SEAR_PIN);
        // Restart sear task
        searOnTask.Reset();
    }
}

static void searDrop()
{
    // Reactivate Sear
    output_low(CYCLE_PORT, SEAR_PIN);
}

static void pneumaticsCocking()
{
    // Start cocking pneumatics
    output_high(CYCLE_PORT, PNEU_PIN);

    // Are we using eyes?
    if(is_bit_set(g_CycleValues.flags, CF_Use_Eyes))
        eyeCycleTask.Reset(); // Start testing eyes
    else
        pneuOnTask.Reset(); // Start pneumatics count down
}

static void pneumaticsCocked()
{
    // Marker is cocked, switch pneumatics off
    output_low(CYCLE_PORT, PNEU_PIN);
    pneuOffTask.Reset(); // wait for pneumatics to come to rest
}

static void cycleComplete()
{
    // If we have any more shots to fire (burst) or we are in Auto mode with the trigger down
    bool restartCycle = g_CycleValues.shotsToGo > 0;

#if defined AUTO_ALLOWED
    restartCycle = restartCycle || (is_bit_set(g_CurrentMode.actionType, AT_Auto) && is_bit_set(g_CycleValues.flags, CF_Trigger_Pressed));
#endif

    if(restartCycle)
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

/// -------------------------------------------------------------------------------
/// Timer Ticks
/// -------------------------------------------------------------------------------

///
/// Every Second Tick
static void onSecondTick()
{
    g_Marker.GetUI()->OnSecond();

#if defined SERIAL_DEBUG
    //Serial.println("Pull Count(/s): " + g_triggerPullCount);
    //Serial.println("Current BPS: " + g_ballShotCount);
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

#if defined GAME_TIMER
///
/// Game Timer Lapsed Callback
void onGameLapsed()
{
}

///
/// Alarm Timer Lapsed Callback
void onAlarmLapsed()
{
}
#endif


///
///
///

void Marker::Init()
{

    // Setup pin direction
    set_input(CYCLE_PORT_REG, TRIGGER_PIN);
    set_output(CYCLE_PORT_REG, SEAR_PIN);
    set_output(CYCLE_PORT_REG, PNEU_PIN);

    set_output(EYE_PORT_REG, IRED_PIN);
    set_input(EYE_PORT_REG, EYE_PIN);

#if defined KEEP_ALIVE_ACTIVE
    // Set pins for Keep alive LED
    // Just to prove the loop is ticking over
    set_output(KEEP_ALIVE_PORT_REG, KEEP_ALIVE_PIN);
    set_output(KEEP_ALIVE_PORT_REG, TRIGGER_PRESSED_PIN);
#endif

    set_input(INPUT_PORT_REG, UP_BUTTON_PIN);
    set_input(INPUT_PORT_REG, OK_BUTTON_PIN);
    set_input(INPUT_PORT_REG, DN_BUTTON_PIN);

    // Enable internal pullups
    output_high(CYCLE_PORT, TRIGGER_PIN);

    output_high(INPUT_PORT, UP_BUTTON_PIN);
    output_high(INPUT_PORT, OK_BUTTON_PIN);
    output_high(INPUT_PORT, DN_BUTTON_PIN);

    output_high(EYE_PORT, IRED_PIN);

    // stop interrupts
    cli();

    // Init timers
    timer_init();
    adc_init();
    i2c_init();

    memcpy_P(&g_CurrentMode, &g_Modes[g_Settings.currentMode], sizeof(MarkerProfile));

    // Setup the tasks
    // 1 second - in increments of 0.1ms
    secondTickTask.SetIntervalTime(10000, true);

    // TODO: Get initial trigger state here?
    triggerChangeTask.SetDebounce(g_Settings.debounceTime);

    // Cycle timings - might need to move these to be configurable
    searOnTask.SetIntervalTime(g_Settings.timings.searOn);
    pneuDelayTask.SetIntervalTime(g_Settings.timings.pneuDel);
    pneuOnTask.SetIntervalTime(g_Settings.timings.pneuOn);
    pneuOffTask.SetIntervalTime(g_Settings.timings.pneuOff);

    eyeCycleTask.SetTaskValues(g_Settings.eyeSettings.eyeTimeout,
                               g_Settings.eyeSettings.detectionTime,
                               g_Settings.eyeSettings.eyeBall);

    // Kick off the initial tasks
    triggerChangeTask.Reset();
    secondTickTask.Reset();

#if defined KEEP_ALIVE_ACTIVE
    keepAliveTask.SetIntervalTime( KEEP_ALIVE_PULSE, true);
    keepAliveTask.Reset();
#endif

    // start interrupts
    sei();

    m_UI.Init();

    adc_start_read( 0 );
}

void Marker::IdleLoop()
{
    m_UI.Update();
}

void Marker::TimerTick()
{
    triggerChangeTask.Update();
    secondTickTask.Update();
    searOnTask.Update();
    pneuDelayTask.Update();
    pneuOnTask.Update();
    pneuOffTask.Update();
    eyeCycleTask.Update();

    m_UI.UpdateControls();
}

void Marker::UpdateEye(uint16_t eyeValue)
{
    eyeCycleTask.SetCurrentEye(eyeValue);
}
