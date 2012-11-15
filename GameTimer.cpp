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

#include "GameTimer.h"

void Timer::SubtractSecond()
{
    if(m_Seconds == 0)
    {
        SubtractMinute();
        m_Seconds = 59;
    }
    else
        m_Seconds--;
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

const char* Timer::ToString()
{
    sprintf(stringBuffer, "%02d:%02d:%02d\0", m_Hours, m_Minutes, m_Seconds);

    return stringBuffer;
}

GameTimer::GameTimer(void)
{
    m_OnAlarm = 0;
    m_Enabled = false;
}

GameTimer::~GameTimer(void)
{
}

void GameTimer::SubtractMinute()
{
    m_GameTimer.SubtractMinute();
}

void GameTimer::SubtractHour()
{
    m_GameTimer.SubtractHour();
}

void GameTimer::StartTimer()
{
    m_Enabled = true;
}

void GameTimer::SetGameTimer(uint8_t hours, uint8_t mins, uint8_t secs, AlarmTimerCallback timerCallback)
{
    m_GameTimer.m_Hours = hours;
    m_GameTimer.m_Minutes = mins;
    m_GameTimer.m_Seconds = secs;

    m_OnTimer = timerCallback;
}

void GameTimer::SetAlarmTimer(uint8_t hours, uint8_t mins, uint8_t secs, AlarmTimerCallback alarmCallback)
{
    m_AlarmTimer.m_Hours = hours;
    m_AlarmTimer.m_Minutes = mins;
    m_AlarmTimer.m_Seconds = secs;

    m_OnAlarm = alarmCallback;
}

void GameTimer::SubtractSecond()
{
    if(!m_Enabled) return;

    m_GameTimer.SubtractSecond();

    if(m_GameTimer.m_Hours == 0 && m_GameTimer.m_Minutes == 0
        && m_GameTimer.m_Seconds == 0 && m_OnTimer != 0)
    {
        m_OnTimer();
        m_Enabled = false;
    }
    else if(m_GameTimer.m_Hours == m_AlarmTimer.m_Hours && m_GameTimer.m_Minutes == m_AlarmTimer.m_Minutes 
        && m_GameTimer.m_Seconds == m_AlarmTimer.m_Seconds&& m_OnAlarm != 0)
    {
        m_OnAlarm();
    } 
}

const char* GameTimer::GameTimerToString()
{
    return m_GameTimer.ToString();
}

const char* GameTimer::AlarmTimerToString()
{
    return m_AlarmTimer.ToString();
}

