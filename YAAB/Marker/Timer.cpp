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

#include "Timer.h"

Timer::Timer(TimerCallback timerCallback, TimerCallback alarmCallback)
{
    m_OnTimer = timerCallback;
    m_OnAlarm = alarmCallback;

    m_SetTime.hours = 0;
    m_SetTime.minutes = 5;
    m_SetTime.seconds = 0;
    
    m_Alarm.hours = 0;
    m_Alarm.minutes = 1;
    m_Alarm.seconds = 0;

    Reset();
}

Timer::~Timer(void)
{
}

void Timer::SetTimer(TimeStruct timer)
{
    m_SetTime = timer;
	
    Reset();
}

void Timer::SetAlarm(TimeStruct alarm)
{
    m_Alarm = alarm;
}

void Timer::Reset() 
{        
    m_Timer = m_SetTime;
     
    m_Enabled = false; 
}

void Timer::ToString(char * stringBuffer)
{
    sprintf(stringBuffer, "%02d:%02d:%02d", m_Timer.hours, m_Timer.minutes, m_Timer.seconds);
}

void Timer::SubtractSecond()
{
    if(m_Timer.seconds == 0)
    {
        SubtractMinute();
        m_Timer.seconds = 59;
    }
    else
        m_Timer.seconds--;

	if (m_Enabled) {
		if (m_Timer.hours == 0 && m_Timer.minutes == 0 && m_Timer.seconds == 0 && m_OnTimer != 0)
		{
			m_OnTimer();

			Reset();
		}
		else if (m_Timer.hours == m_Alarm.hours && m_Timer.minutes == m_Alarm.minutes && m_Timer.seconds == m_Alarm.seconds && m_OnAlarm != 0)
		{
			m_OnAlarm();
		}
	}
}

void Timer::SubtractMinute()
{
    if(m_Timer.minutes == 0)
    {
        SubtractHour();
        m_Timer.minutes = 59;
    }
    else
        m_Timer.minutes--;
}

void Timer::SubtractHour()
{
    if(m_Timer.hours > 0)
        m_Timer.hours--;
}

