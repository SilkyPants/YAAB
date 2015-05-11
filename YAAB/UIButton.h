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

#include "common.h"

enum UIButtonStates
{
    UBS_Up,
    UBS_Down,
    UBS_Held
};

class UIButton
{
public:
    void Init(volatile uint8_t *pinPort, uint8_t pinBit);
    
    void UpdateState();
    UIButtonStates GetButtonState() { return m_CurrentState; }
    
private:
    volatile uint8_t* m_Port;
    uint8_t m_Pin;
    bool    m_LastState;
    
    UIButtonStates  m_CurrentState;
    uint16_t        m_DebounceCounter;
};
