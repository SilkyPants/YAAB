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

#include "IntervalLapse.h"

IntervalLapse::IntervalLapse(TaskConditionMet conditionMet) : Task(conditionMet) 
{ 
}

IntervalLapse::~IntervalLapse(void) 
{ 
}

bool IntervalLapse::IsConditionMet()
{
    return m_Interval == 0 && m_Enabled;
}

void IntervalLapse::UpdateInternal()
{
    m_Interval--;
}

void IntervalLapse::ConditionMet()
{
    if(m_AutoReset)
        Reset();
}

void IntervalLapse::SetIntervalTime(uint16_t intervalTime, bool autoReset)
{
    m_Interval = m_IntervalReset = intervalTime;
    m_AutoReset = autoReset;
}

void IntervalLapse::Reset() 
{
    Task::Reset();
    m_Interval = m_IntervalReset; 
}

