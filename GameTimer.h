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

    void SubtractSecond();
    void SubtractMinute();
    void SubtractHour();

    const char* ToString();
};

class GameTimer
{
    bool m_Enabled;
    Timer m_GameTimer;
    Timer m_AlarmTimer;
    AlarmTimerCallback m_OnTimer;
    AlarmTimerCallback m_OnAlarm;

    void SubtractMinute();
    void SubtractHour();

public:
    GameTimer(void);
    ~GameTimer(void);

    void StartTimer();

    void SetGameTimer(uint8_t hours, uint8_t mins, uint8_t secs, AlarmTimerCallback timerCallback);
    void SetAlarmTimer(uint8_t hours, uint8_t mins, uint8_t secs, AlarmTimerCallback alarmCallback);

    void SubtractSecond();

    const char* GameTimerToString();
    const char* AlarmTimerToString();
};

