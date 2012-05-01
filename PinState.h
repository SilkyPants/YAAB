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
#include "IntervalLapse.h"

class PinState :
    public Task
{
private:
    char m_Port;
    char m_Pin;
    bool m_State;

    inline bool IsConditionMet()
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

class PinChange : public Task
{
protected:
    volatile uint8_t* m_Port;
    uint8_t m_Pin;
    uint8_t m_Debounce;
    uint8_t m_InitialDebounce;
    bool m_State;
    bool m_LastValidState;

    bool IsConditionMet()
    {
        return m_Debounce <= 0 && m_Enabled && m_LastValidState == m_State;
    }

    bool UpdatePinState()
    {
        if(input_value(*m_Port, m_Pin) != m_State)
        {
            m_Debounce = m_InitialDebounce;
            m_State = input_value(*m_Port, m_Pin);
            
            if(!m_Enabled)
              m_Enabled = true;

            return true;
        }

        return false;
    }
    
    void UpdateInternal()
    {
        if(!UpdatePinState())
            m_Debounce--;
    }

    void PostUpdate()
    {
        if(IsConditionMet())
        {
            m_LastValidState = !m_State;
        }
    }

public:

    PinChange(TaskConditionMet conditionMet, volatile uint8_t *pinPort, char pinBit, char debounce = 0) : Task(conditionMet)
    {
        m_State = input_value(*m_Port, m_Pin);
        m_Debounce = m_InitialDebounce = debounce;
    }

    virtual ~PinChange(void) { }
};

class PinDrive : public IntervalLapse
{
protected:
    uint8_t m_Port;
    uint8_t m_Pin;
    uint16_t m_Delay;
    uint16_t m_DelayReset;

    bool IsConditionMet()
    {
        return m_Delay == 0 && IntervalLapse::IsConditionMet();
    }
    
    void UpdateInternal()
    {
        if(m_Delay == 0)
        {
            output_high(m_Port, m_Pin);
            IntervalLapse::UpdateInternal();
        }
        else
            m_Delay--;
    }

    void PostUpdate()
    {
        if(IsConditionMet())
        {
            output_low(m_Port, m_Pin);
        }
    }

public:

    PinDrive(TaskConditionMet conditionMet, uint8_t pinPort, char pinBit, uint16_t delay, uint16_t interval) : IntervalLapse(conditionMet, interval, false)
    {
        m_Port = pinPort;
        m_Pin = pinBit;
        m_Delay = m_DelayReset = delay;
    }

    virtual ~PinDrive(void) { }

    void Reset() 
    {
        IntervalLapse::Reset();
        m_Delay = m_DelayReset; 
    }
};

