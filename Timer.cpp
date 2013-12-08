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

#include "Timer.h"

Timer::Timer(TimerCallback timerCallback)
{
    m_OnTimer = timerCallback;

    m_SetHours = 0;
    m_SetMinutes = 5;
    m_SetSeconds = 0;

    Reset();
}

Timer::~Timer(void)
{
}

void Timer::SetTimer(uint8_t hours, uint8_t mins, uint8_t secs)
{
    m_SetHours = hours;
    m_SetMinutes = mins;
    m_SetSeconds = secs;

    Reset();
}

void Timer::Reset() 
{        
    m_Hours = m_SetHours;
    m_Minutes = m_SetMinutes;
    m_Seconds = m_SetSeconds;
     
    m_Enabled = false; 
}

const char* Timer::ToString()
{
    sprintf(stringBuffer, "%02d:%02d:%02d", m_Hours, m_Minutes, m_Seconds);

    return stringBuffer;
}

void Timer::SubtractSecond()
{
    if(m_Seconds == 0)
    {
        SubtractMinute();
        m_Seconds = 59;
    }
    else
        m_Seconds--;

    if(m_Hours == 0 && m_Minutes == 0 && m_Seconds == 0 && m_OnTimer != 0)
    {
        m_OnTimer();

        Reset();
    }
}

void Timer::SubtractMinute()
{
    if(m_Minutes == 0)
    {
        SubtractHour();
        m_Minutes = 59;
    }
    else
        m_Minutes--;
}

void Timer::SubtractHour()
{
    if(m_Hours > 0)
        m_Hours--;
}

