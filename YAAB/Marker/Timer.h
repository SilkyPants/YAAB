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

#pragma once

#include "../common.h"

#define MAX_TIMER_HOURS 15

struct TimeStruct 
{
	uint16_t hours	 : 4;
	uint16_t minutes : 6;
	uint16_t seconds : 6;
	
	void Zero()
	{
		hours = 0;
		minutes = 0;
		seconds = 0;
	}
};

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
    TimeStruct m_SetTime;
    
    ///
    /// Current time left on timer
    TimeStruct m_Timer;
    
    ///
    /// Current time left on timer
    TimeStruct m_Alarm;
    
    ///
    /// Stored string buffer to make it easier to print to string
    //char stringBuffer[9];
    
    ///
    /// Function pointer to call once timer has lapsed
    TimerCallback m_OnTimer;
    
    ///
    /// Function pointer to call once alarm has lapsed
    TimerCallback m_OnAlarm;
    
    ///
    /// Subtracts a minute from the timer
    void SubtractMinute();
    
    ///
    /// Subtracts an hour from the timer
    void SubtractHour();

public:
    
    ///
    /// Constructor/Destructor
    Timer(TimerCallback timerCallback, TimerCallback alarmCallback);
    ~Timer(void);
    
    ///
    /// Set the initial timer
    void SetTimer(TimeStruct timer);
    
    ///
    /// Set the initial timer
    void SetAlarm(TimeStruct alarm);
    
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
    void ToString(char * stringBuffer);
};


