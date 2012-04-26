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

#include "flags.h"

struct EyeSettings // 4 bytes
{
    unsigned char detectionTime;            // Time before eye state change triggered
    unsigned char eyeBall;                  // Value for ball seen
    unsigned int eyeTimeout;                // How long to wait for the eyes before closing
};

struct MarkerTiming // 6 bytes
{
    unsigned char searOn;                   // How long the hammer sear is down for
    unsigned char pneuDel;                  // How long to wait from the start of the cycle before cocking the marker
    unsigned int pneuOn;                    // How long to open the cocking solenoid
    unsigned int pneuOff;                   // How long after cocking the marker before we can fire again
};

struct MarkerSettings // 16 bytes
{
    unsigned char debounceTime;             // How long before saying the trigger is pressed/released
    unsigned char currentProfile;           // The current profile in use
    unsigned long shotsSinceLastReset;      // Shot counter
    MarkerTiming timings;                   // Settings for the marker pneumatics cycle
    EyeSettings eyeSettings;                // Settings for the eyes
};

struct CycleValues // 4 bytes?
{
    unsigned char flags : 4;                // Flags using in marker cycle (CycleFlags)
    unsigned char markerState : 2;          // Marker current state
    unsigned char eyesState : 2;            // Eyes current state
    unsigned char shotsToGo;                // Shots to fire in 'cycle' (Burst)
    unsigned int cycleCount;                // Cycle counter in 0.1ms increments (max 6.5535 seconds)
};

struct MarkerProfile // 8 bytes
{
    char profileName[6];                    // Name for the profile
    unsigned char shotsToFirePress : 4;     // Burst fire on press (max 15)
    unsigned char shotsToFireRelease : 4;   // Burst fire on release (max 15)
    unsigned char actionType : 4;           // Pump, Semi, Auto, Training
    unsigned char triggerAction : 2;        // What to do on trigger state change
    unsigned char : 0;                      // Padding
};
