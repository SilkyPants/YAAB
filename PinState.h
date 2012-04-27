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

class PinState :
    public Task
{
private:
    char m_Port;
    char m_Pin;
    bool m_State;

    bool IsConditionMet()
    {
        return input_value(m_Port, m_Pin) == m_State;
    }

public:

    PinState(TaskConditionMet conditionMet, char pinPort, char pinBit, bool pinState) : Task(conditionMet)
    {
        m_Port = pinPort;
        m_Pin = pinBit;
        m_State = pinState;
    }

    virtual ~PinState(void) {}
};

class PinChange :
    public Task
{
private:
    volatile uint8_t* m_Port;
    char m_Pin;
    uint8_t m_State;
    uint8_t m_LastValidState;
    int m_Debounce;
    int m_InitialDebounce;

    bool IsConditionMet()
    {
        return m_Debounce <= 0 && m_Enabled && m_LastValidState == m_State;
    }
    
    void UpdateInternal(int delta)
    {

        if(input_value(*m_Port, m_Pin) != m_State)
        {
            m_Debounce = m_InitialDebounce;
            m_State = input_value(*m_Port, m_Pin);
            
            if(!m_Enabled)
              m_Enabled = true;
        }
        else if(m_Enabled && m_Debounce > 0)
        {
            m_Debounce -= delta;
        }
    }

public:

    PinChange(TaskConditionMet conditionMet, volatile uint8_t *pinPort, char pinBit, int debounce = 0) : Task(conditionMet)
    {
        m_Port = pinPort;
        m_Pin = pinBit;
        m_State = input_value(*m_Port, m_Pin);
        m_Debounce = m_InitialDebounce = debounce;
    }

    virtual ~PinChange(void) {}
    
    void Update(int delta)
    {
        Task::Update(delta);
        
        if(IsConditionMet())
        {
            m_Enabled = false;
            m_LastValidState = !m_State;
        }
    }
};

