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

#pragma once

#include "common.h"

///
/// Typedef for function pointer. This is used when a timer has lapsed.
typedef void (*TimerCallback)();

///
/// Counts down a certain time, and can call back to a function once its lapsed.
class Timer
{
    ///
    /// Is the timer enabled?
    bool m_Enabled;
    
    ///
    /// The set time, this is used to reset the timer
    uint8_t m_SetHours;
    uint8_t m_SetMinutes;
    uint8_t m_SetSeconds;
    
    ///
    /// Current time left on timer
    uint8_t m_Hours;
    uint8_t m_Minutes;
    uint8_t m_Seconds;
    
    ///
    /// Stored string buffer to make it easier to print to string
    char stringBuffer[9];
    
    ///
    /// Function pointer to call once timer has lapsed
    TimerCallback m_OnTimer;
    
    ///
    /// Subtracts a minute from the timer
    void SubtractMinute();
    
    ///
    /// Subtracts an hour fron the timer
    void SubtractHour();

public:
    
    ///
    /// Constructor/Destructor
    Timer(TimerCallback timerCallback);
    ~Timer(void);
    
    ///
    /// Set the initial timer
    void SetTimer(uint8_t hours, uint8_t mins, uint8_t secs);
    
    ///
    /// Subtracts a second from the timer, and responsible for triggering Timer Callback
    void SubtractSecond();
    
    ///
    /// Start/Stop the timer
    void StartTimer() { m_Enabled = true; }
    void StopTimer() { m_Enabled = false; }
    
    ///
    /// Resets the timer back to it's default settings
    void Reset();

    ///
    /// Returns the timer left on the timer in HH:MM:SS format
    const char* ToString();
};

