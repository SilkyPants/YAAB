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

#include "UIButton.h"

#define INPUT_DEBOUNCE 10
#define INPUT_HELD 10000

void UIButton::Init(volatile uint8_t *pinPort, uint8_t pinBit)
{
    m_Port = pinPort;
    m_Pin = pinBit;
    m_LastState = input_value(*m_Port, m_Pin);
}

void UIButton::UpdateState()
{
    bool currState = input_value(*m_Port, m_Pin);
    
    if (currState != m_LastState) {
        m_DebounceCounter = 0;
    }
    else {
    
        if (currState && m_CurrentState != UBS_Up) {
            m_DebounceCounter++;
            
            if (m_DebounceCounter > INPUT_DEBOUNCE) {
                m_DebounceCounter = 0;
                m_CurrentState = UBS_Up;
            }
        }
        else if(!currState) {
            
            if (m_CurrentState == UBS_Up) {
                m_DebounceCounter++;
                
                if (m_DebounceCounter > INPUT_DEBOUNCE) {
                    m_DebounceCounter = 0;
                    m_CurrentState = UBS_Down;
                }
            }
            else if (m_CurrentState == UBS_Down) {
                m_DebounceCounter++;
                
                if (m_DebounceCounter > INPUT_HELD) {
                    m_DebounceCounter = 0;
                    m_CurrentState = UBS_Held;
                }
            }
        }
    }
    
    m_LastState = currState;
}