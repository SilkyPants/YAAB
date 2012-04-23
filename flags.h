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
enum ProfileFlags
{
    PF_Pump             = 0x01,
    PF_Semi             = 0x02,
    PF_Auto             = 0x04,
    PF_FireOnPress      = 0x08,
    PF_FireOnRelease    = 0x10,
    PF_CappedROF        = 0x20,
    PF_Reserved1        = 0x40,
    PF_Reserved2        = 0x80,
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
    CS_Ready_To_Fire    = 0x01,
    CS_Sear_Firing      = 0x02,
    CS_Breech_Opening   = 0x04,
    CS_Breech_Closing   = 0x08,
};

/// State of the marker eyes
enum EyeStates
{
    ES_Empty_Seen       = 0x01,
    ES_Ball_Seen        = 0x02,
    ES_Bolt_Seen        = 0x04,
    ES_Reserved         = 0x08,
};