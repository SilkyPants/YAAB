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

/// Function prototype for callbacks on tasks
typedef void (*TaskConditionMet)();

class Task
{
private:
    TaskConditionMet onConditionMet;
    
protected:
    bool m_Enabled;

    virtual bool IsConditionMet() = 0;
    virtual void UpdateInternal() = 0;
    virtual void ConditionMet() { }

public:
    Task(TaskConditionMet conditionMet)
    {
        onConditionMet = conditionMet;
    }

    virtual ~Task(void) { }

    virtual void Update()
    {
        if(!m_Enabled) return;

        UpdateInternal();

        if(IsConditionMet())
        {
            m_Enabled = false;

            if(onConditionMet != 0)
                onConditionMet();
            
            ConditionMet();
        }
    }

    void Stop() {  m_Enabled = false; }

    virtual void Reset() {  m_Enabled = true; }
};
