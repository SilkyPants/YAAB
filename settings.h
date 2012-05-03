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

struct CycleValues // 2 bytes
{
    unsigned char flags;                    // Flags using in marker cycle (CycleFlags)
    unsigned char shotsToGo;                // Shots to fire in 'cycle' (Burst)
};

struct MarkerProfile // 16 bytes
{
    char profileName[13];                   // Name for the profile
    unsigned char shotsToFirePress : 4;     // Burst fire on press (max 15)
    unsigned char shotsToFireRelease : 4;   // Burst fire on release (max 15)
    unsigned char actionType : 4;           // Pump, Semi, Auto
    unsigned char triggerAction : 2;        // What to do on trigger state change
    unsigned char rofCapped : 1;            // Is the ROF capped
    unsigned char : 0;                      // Padding
    unsigned char rofCap : 5;               // Is the ROF capped
    unsigned char : 0;                      // Padding
};
