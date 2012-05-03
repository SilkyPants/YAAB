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

#include "Task.h"

class IntervalLapse : public Task
{
protected:
    uint16_t m_Interval;
    uint16_t m_IntervalReset;
    bool m_AutoReset;

    bool IsConditionMet()
    {
        return m_Interval == 0 && m_Enabled;
    }

    void UpdateInternal()
    {
        m_Interval--;
    }

    void ConditionMet()
    {
        if(m_AutoReset)
            Reset();
    }

public:

    IntervalLapse(TaskConditionMet conditionMet) : Task(conditionMet) { }

    void SetIntervalTime(uint16_t intervalTime, bool autoReset = false)
    {
        m_Interval = m_IntervalReset = intervalTime;
        m_AutoReset = autoReset;
    }

    ~IntervalLapse(void) { }

    void Reset() 
    {
        Task::Reset();
        m_Interval = m_IntervalReset; 
    }
};

