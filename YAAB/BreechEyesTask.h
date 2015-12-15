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

#include "Task.h"

class BreechEyesTask : public Task
{
private:
    uint8_t m_DetectCount;
    uint8_t m_DetectTime;
    uint16_t m_CurrentEye;
    uint16_t m_BallDetect;
    uint16_t m_Timeout;
    uint16_t m_TimeoutCount;
    bool m_EmptySeen;
    bool m_BallSeen;

    bool UpdateEye();
    void UpdateInternal();
    bool IsConditionMet();

public:
    BreechEyesTask(TaskConditionMet conditionsMet);
    ~BreechEyesTask(void);

    void SetTaskValues(uint16_t timeout, uint8_t detectTime, uint16_t ballDetect);
    void SetCurrentEye(uint16_t currentEye);
    uint16_t GetCurrentEye() { return m_CurrentEye; };
    void Reset();
};


