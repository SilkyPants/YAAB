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

typedef void (*TimerCallback)();

class Timer
{
    bool m_Enabled;

    uint8_t m_SetHours;
    uint8_t m_SetMinutes;
    uint8_t m_SetSeconds;

    uint8_t m_Hours;
    uint8_t m_Minutes;
    uint8_t m_Seconds;

    char stringBuffer[9];

    TimerCallback m_OnTimer;

    void SubtractMinute();
    void SubtractHour();

public:
    Timer(void);
    ~Timer(void);

    void SubtractSecond();
    void SetTimer(uint8_t hours, uint8_t mins, uint8_t secs, TimerCallback timerCallback);
    
    void StartTimer() { m_Enabled = true; }
    void StopTimer() { m_Enabled = false; }
    void Reset();

    const char* ToString();
};

