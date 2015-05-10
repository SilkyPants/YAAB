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

#include "crius_oled.h"

enum MenuStates
{
    GameScreen,
    MenuRoot,
};

class UserInterface
{
public:
    void Init();
    void OnSecond();
    void Update();
    void Draw();
    
private:
    MenuStates m_CurrentState;
    CRIUS_OLED m_Display;
    
    uint8_t battLevel;
    uint8_t eyeAnimIdx;
    
    void SetState(MenuStates p_NewState);
    void SetHeaderText(const char* const* string);
    void DrawBatteryLevel(uint8_t battPercent);
    
    void GameScreen_OnEnter();
    void GameScreen_Update();
    void MenuRoot_OnEnter();
    void MenuRoot_Update();
    
};
