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

#include "UserInterface.h"

#include "settings.h"
#include "BreechEyesTask.h"

extern BreechEyesTask eyeCycleTask;
extern MarkerProfile g_CurrentMode;

void UserInterface::GameScreen_OnEnter()
{
    SetHeaderText(&(ModeHeaderStrings[g_CurrentMode.profileNameIndex]));
}

void UserInterface::GameScreen_Update()
{
    char buffer[16];
    
//    long eye = eyeCycleTask.GetCurrentEye();
//    eye *= 100;
//    eye /= 1024;
//    
//    sprintf(buffer, "%03ld", eye);
//    
//    m_Display.DrawString(3, 17, buffer, BPS_FONT);
//    
//    sprintf(buffer, "CURR EYE: %04u", eyeCycleTask.GetCurrentEye());
//    
    //    m_Display.DrawString(3, 39, buffer);
    
    sprintf(buffer, "UP BTN: %d", m_UpButton.GetButtonState());
    
    m_Display.DrawString(3, 17, buffer);
    
    sprintf(buffer, "OK BTN: %d", m_OkButton.GetButtonState());
    
    m_Display.DrawString(3, 24, buffer);
    
    sprintf(buffer, "DN BTN: %d", m_DnButton.GetButtonState());
    
    m_Display.DrawString(3, 31, buffer);
}