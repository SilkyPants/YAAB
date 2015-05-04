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

#include "PinState.h"

bool PinState::IsConditionMet()
{
    return input_value(m_Port, m_Pin) == m_State;
}

PinState::PinState(TaskConditionMet conditionMet, char pinPort, char pinBit, bool pinState) : Task(conditionMet)
{
    m_Port = pinPort;
    m_Pin = pinBit;
    m_State = pinState;
}

PinState::~PinState(void) {}

bool PinChange::IsConditionMet()
{
    return m_Debounce <= 0 && m_Enabled && m_LastValidState == m_State;
}

bool PinChange::UpdatePinState()
{
    if(input_value(*m_Port, m_Pin) != m_State)
    {
        m_Debounce = m_InitialDebounce;
        m_State = input_value(*m_Port, m_Pin);

        return true;
    }

    return false;
}
    
void PinChange::UpdateInternal()
{
    if(!UpdatePinState() && m_Debounce > 0)
        m_Debounce--;
}

void PinChange::ConditionMet()
{
    m_LastValidState = !m_State;
    m_Enabled = true;
}

PinChange::PinChange(TaskConditionMet conditionMet, volatile uint8_t *pinPort, uint8_t pinBit) : Task(conditionMet)
{
    m_Port = pinPort;
    m_Pin = pinBit;
    m_State = input_value(*m_Port, m_Pin);
}

PinChange::~PinChange(void) { }

void PinChange::SetDebounce(uint8_t debounce)
{
    m_Debounce = m_InitialDebounce = debounce;
}


