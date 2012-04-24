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

/// Flags for user profiles
enum ActionTypes
{
    AT_Training         = 0x1,
    AT_Pump             = 0x2,
    AT_Semi             = 0x4,
    AT_Auto             = 0x8,
};

enum TriggerActions
{
    TA_FireOnPress      = 0x1,
    TA_FireOnRelease    = 0x2,
};

/// Flags used in a marker cycle
enum CycleFlags
{
    CF_Use_Eyes         = 0x01,
    CF_Trigger_Pressed  = 0x02,
    CF_Debounce_Charge  = 0x04,
    CF_Reserved         = 0x08,
};

/// The state of a marker cycle
enum CycleStates
{
    CS_Ready_To_Fire    = 0x0,
    CS_Sear_Firing      = 0x1,
    CS_Breech_Opening   = 0x2,
    CS_Breech_Closing   = 0x3
};

/// State of the marker eyes
enum EyeStates
{
    ES_Empty_Seen       = 0x1,
    ES_Ball_Seen        = 0x2,
    ES_Bolt_Seen        = 0x4,
};