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

#include "task.h"

class BreechEyesTask : public Task
{
private:
    uint8_t m_DetectCount;
    uint8_t m_DetectTime;
    uint8_t m_BallDetect;
    uint8_t m_CurrentEye;
    uint16_t m_Timeout;
    uint16_t m_TimeoutCount;
    bool m_EmptySeen;
    bool m_BallSeen;

    bool UpdateEye()
    {
        if(!m_EmptySeen)
        {
            if(m_CurrentEye >= m_BallDetect)
            {
                m_DetectCount = m_DetectTime;
                return false;
            }
        }
        else
        {
            if(m_CurrentEye < m_BallDetect)
            {
                m_DetectCount = m_DetectTime;
                return false;
            }
        }

        return true;
    }

    void UpdateInternal()
    {
        if(UpdateEye())
            m_DetectCount--;

        if(m_DetectCount == 0)
        {
            if(m_EmptySeen)
                m_BallSeen = true;
            else
                m_EmptySeen = true;
        }

        m_TimeoutCount--;
    }

    bool IsConditionMet()
    {
        return ((m_EmptySeen && m_BallSeen) || m_TimeoutCount == 0) && m_Enabled;
    }

public:

    BreechEyesTask(TaskConditionMet conditionsMet) : Task(conditionsMet) { }

    void SetTaskValues(uint16_t timeout, uint8_t detectTime, uint8_t ballDetect)
    {
        m_DetectCount = m_DetectTime = detectTime;
        m_BallDetect = ballDetect;
        m_TimeoutCount = m_Timeout = timeout;
        m_EmptySeen = false;
        m_BallSeen = false;
    }

    void SetCurrentEye(uint8_t currentEye)
    {
        m_CurrentEye = currentEye;
    }

    virtual ~BreechEyesTask(void)
    {
    }

    void Reset()
    {
        m_TimeoutCount = m_Timeout;
        m_DetectCount = m_DetectTime;
        m_EmptySeen = false;
        m_BallSeen = false;
    }
};

