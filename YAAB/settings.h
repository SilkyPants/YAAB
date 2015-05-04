/*
YAAB - Yet Another Autococker Board
Arduino based autococker board developed around the platform and ATMEL AVR 
chips

Copyright (C) 2013  Dan Silk

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

#pragma once

#include "flags.h"

/*
+------------------------+-----------+-----------+-----------+
| Value                  |    Min    |    Def    |    Max    |
+------------------------+-----------+-----------+-----------+
| Sear On (SON)          |   0.5ms   |   4.0ms   |   4.0ms   |---+
+------------------------+-----------+-----------+-----------+   |
| Pneumatic Delay (PDEL) |   4.1ms   |   6.0ms   |  14.9ms   |   |
+------------------------+-----------+-----------+-----------+   +---> Marker Timing
| Pneumatic On (PON)     |    20ms   |    55ms   |   199ms   |   |
+------------------------+-----------+-----------+-----------+   |
| Pneumatic Off (POFF)   |    20ms   |    55ms   |   199ms   |---+
+------------------------+-----------+-----------+-----------+
| Eye Timeout (EOUT)     |    0.1s   |    1.0s   |   19.9s   |---+
+------------------------+-----------+-----------+-----------+   |
| Eye Ball Detect (BALD) |     1ms   |     1ms   |   19ms    |   +---> Eye Settings
+------------------------+-----------+-----------+-----------+   |
| Eye Empty Detect (EMPD)|     1ms   |     2ms   |   19ms    |---+
+------------------------+-----------+-----------+-----------+
| Game Timer             |  0:00:00  |  0:05:00  |  5:00:00  |
+------------------------+-----------+-----------+-----------+
| Alarm Timer            |  0:00:00  |  0:01:00  |  5:00:00  |
+------------------------+-----------+-----------+-----------+
*/

struct EyeSettings // 4 bytes
{
    uint8_t detectionTime;            // Time before eye state change triggered
    uint8_t eyeBall;                  // Value for ball seen
    uint16_t eyeTimeout;              // How long to wait for the eyes before closing
};

struct MarkerTiming // 6 bytes
{
    uint8_t searOn;                   // How long the hammer sear is down for
    uint8_t pneuDel;                  // How long to wait from the start of the cycle before cocking the marker
    uint16_t pneuOn;                  // How long to open the cocking solenoid
    uint16_t pneuOff;                 // How long after cocking the marker before we can fire again
};

struct MarkerSettings // 16 bytes
{
    uint8_t debounceTime;             // How long before saying the trigger is pressed/released
    uint8_t currentProfile;           // The current profile in use
    uint32_t shotsSinceLastReset;     // Shot counter
    MarkerTiming timings;             // Settings for the marker pneumatics cycle
    EyeSettings eyeSettings;          // Settings for the eyes
};

struct CycleValues // 2 bytes
{
    uint8_t flags;                    // Flags using in marker cycle (CycleFlags)
    uint8_t shotsToGo;                // Shots to fire in 'cycle' (Burst)
};

struct MarkerProfile // 16 bytes
{
    char profileName[13];             // Name for the profile
    uint8_t shotsToFirePress : 2;     // Burst fire on press (max 3)
    uint8_t shotsToFireRelease : 2;   // Burst fire on release (max 3)
    uint8_t actionType : 4;           // Pump, Semi, Auto
    uint8_t triggerAction : 2;        // What to do on trigger state change
    uint8_t rofCapped : 1;            // Is the ROF capped
    uint8_t : 0;                      // Padding
    uint8_t rofCap : 6;               // ROF capped value (max 63)
    uint8_t : 0;                      // Padding
};

