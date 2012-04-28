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
    AT_Training,
    AT_Pump,
    AT_Semi,
    AT_Auto,
};

enum TriggerActions
{
    TA_FireOnPress,
    TA_FireOnRelease,
};

/// Flags used in a marker cycle
enum CycleFlags
{
    CF_Use_Eyes,
    CF_Trigger_Pressed,
};

/// The state of a marker cycle
enum CycleStates
{
    CS_Ready_To_Fire,
    CS_Sear_Firing,
    CS_Breech_Opening,
    CS_Breech_Closing
};

/// State of the marker eyes
enum EyeStates
{
    ES_Empty_Seen,
    ES_Ball_Seen,
};
