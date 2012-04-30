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

typedef void (*AlarmTimerCallback)();

struct Timer
{
    uint8_t m_Hours;
    uint8_t m_Minutes;
    uint8_t m_Seconds;
    char stringBuffer[9];

    void SubtractSecond()
    {
        if(m_Seconds == 0)
        {
            SubtractMinute();
            m_Seconds = 59;
        }
        else
            m_Seconds--;
    }

    void SubtractMinute()
    {
        if(m_Minutes == 0)
        {
            SubtractHour();
            m_Minutes = 59;
        }
        else
            m_Minutes--;
    }

    void SubtractHour()
    {
        if(m_Hours > 0)
            m_Hours--;
    }

    const char* ToString()
    {
        sprintf(stringBuffer, "%02d:%02d:%02d", m_Hours, m_Minutes, m_Seconds);

        return stringBuffer;
    }
};

class GameTimer
{
    bool m_Enabled;
    Timer m_GameTimer;
    Timer m_AlarmTimer;
    AlarmTimerCallback m_OnTimer;
    AlarmTimerCallback m_OnAlarm;

    void SubtractMinute()
    {
        m_GameTimer.SubtractMinute();
    }

    void SubtractHour()
    {
        m_GameTimer.SubtractHour();
    }

public:
    GameTimer(void)
    {
        m_OnAlarm = 0;
        m_Enabled = false;
    }

    void StartTimer()
    {
        m_Enabled = true;
    }

    void SetGameTimer(uint8_t hours, uint8_t mins, uint8_t secs, AlarmTimerCallback timerCallback)
    {
        m_GameTimer.m_Hours = hours;
        m_GameTimer.m_Minutes = mins;
        m_GameTimer.m_Seconds = secs;

        m_OnTimer = timerCallback;
    }

    void SetAlarmTimer(uint8_t hours, uint8_t mins, uint8_t secs, AlarmTimerCallback alarmCallback)
    {
        m_AlarmTimer.m_Hours = hours;
        m_AlarmTimer.m_Minutes = mins;
        m_AlarmTimer.m_Seconds = secs;

        m_OnAlarm = alarmCallback;
    }

    void SubtractSecond()
    {
        if(!m_Enabled) return;

        m_GameTimer.SubtractSecond();

        if(m_GameTimer.m_Hours == m_AlarmTimer.m_Hours 
            && m_GameTimer.m_Minutes == m_AlarmTimer.m_Minutes 
            && m_GameTimer.m_Seconds == m_AlarmTimer.m_Seconds
            && m_OnAlarm != 0)
        {
            m_OnAlarm();
        }
        else if(m_GameTimer.m_Hours == 0 
            && m_GameTimer.m_Minutes == 0
            && m_GameTimer.m_Seconds == 0
            && m_OnTimer != 0)
        {
            m_OnTimer();
            m_Enabled = false;
        }
    }

    const char* GameTimerToString()
    {
        return m_GameTimer.ToString();
    }

    const char* AlarmTimerToString()
    {
        return m_AlarmTimer.ToString();
    }
};

