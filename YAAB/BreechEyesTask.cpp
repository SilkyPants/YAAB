/*
YAAB - Yet Another Autococker Board
Arduino based autococker board developed around the platform and ATMEL AVR 
chips

Copyright (C) 2013  Dan Silk

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

#include "BreechEyesTask.h"

BreechEyesTask::BreechEyesTask(TaskConditionMet conditionsMet) : Task(conditionsMet)
{
    m_CurrentEye = 0;
    m_EmptySeen = false;
    m_BallSeen = false;
}

BreechEyesTask::~BreechEyesTask(void)
{
}

bool BreechEyesTask::UpdateEye()
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

void BreechEyesTask::UpdateInternal()
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

bool BreechEyesTask::IsConditionMet()
{
    return ((m_EmptySeen && m_BallSeen) || m_TimeoutCount == 0) && m_Enabled;
}

void BreechEyesTask::SetTaskValues(uint16_t timeout, uint8_t detectTime, uint16_t ballDetect)
{
    m_DetectCount = m_DetectTime = detectTime;
    m_BallDetect = ballDetect;
    m_TimeoutCount = m_Timeout = timeout;
    m_EmptySeen = false;
    m_BallSeen = false;
}

void BreechEyesTask::SetCurrentEye(uint16_t currentEye)
{
    m_CurrentEye = currentEye;
}

void BreechEyesTask::Reset()
{
    m_TimeoutCount = m_Timeout;
    m_DetectCount = m_DetectTime;
    m_EmptySeen = false;
    m_BallSeen = false;
}



