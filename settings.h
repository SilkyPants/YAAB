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
#include "flags.h"

struct EyeSettings // 4 bytes
{
    uint8_t ballDetection;              // Time before ball is detected
    uint8_t emptyDetection;             // Time before empty is detected
    uint8_t eyeBall;                    // Value for ball seen
    uint8_t eyeBolt;                    // Value for bolt seen
};

struct MarkerTiming // 4 bytes?
{
    uint8_t searOn : 6;                 // How long the hammer sear is down for
    uint8_t pneuDel : 6;                // How long to wait from the start of the cycle before cocking the marker
    uint16_t pneuOn : 10;               // How long to open the cocking solenoid
    uint16_t pneuOff : 10;              // How long after cocking the marker before we can fire again
};

struct MarkerSettings // 16 bytes?
{
    MarkerTiming timings;               // Settings for the marker pneumatics cycle
    EyeSettings eyeSettings;            // Settings for the eyes
    uint8_t currentProfile;             // The current profile in use
    uint8_t debounce;                   // How long before saying the trigger is pressed/released
    uint16_t eyeTimeout;                // How long to wait for the eyes
    uint32_t shotsSinceLastReset;       // Shot counter
};

struct CycleValues // 4 bytes?
{
    uint8_t flags : 2;                  // Flags using in marker cycle (CycleFlags)
    uint8_t shotsToGo : 4;              // Shots to fire in 'cycle' (Burst)
    uint8_t markerState : 2;            // Marker current state
    uint8_t eyesState : 2;              // Eyes current state
    uint16_t cycleCount;                // Cycle counter in 0.1ms increments (max 6.5535 seconds)
    uint8_t padding : 6;                // Padding
};

struct MarkerProfile // 8 bytes
{
    char profileName[6];                // Name for the profile
    uint8_t profileFlags : 4;           // Flags used while cycling (ProfileFlags)
    uint8_t shotsToFirePress : 4;       // Burst fire on press (max 15)
    uint8_t shotsToFireRelease : 4;     // Burst fire on release (max 15)
    uint8_t reserved : 4;               // Padding
};
