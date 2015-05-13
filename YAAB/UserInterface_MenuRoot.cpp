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

void UserInterface::MenuRoot_OnEnter()
{
	m_Display.ClearDisplay();
	
    SetHeaderText(&(MenuStrings[MENU_MENU]));
    
    DrawString_P(11, 16, &(MenuStrings[MENU_TIMERS]));
    DrawString_P(11, 23, &(MenuStrings[MENU_DISPLAY]));
    DrawString_P(11, 30, &(MenuStrings[MENU_TRAINING]));
    DrawString_P(11, 37, &(MenuStrings[MENU_SETUP]));
    DrawString_P(11, 44, &(MenuStrings[MENU_SLEEP]));
    DrawString_P(11, 51, &(MenuStrings[MENU_BACK]));

	ResetCursorAndShow(true);
}

void UserInterface::MenuRoot_Update()
{
	if (m_DnButton.GetButtonState() == UBS_Down) {
		ChangeOption(false, 6);
    }
    
    
    if (m_UpButton.GetButtonState() == UBS_Down) {
        ChangeOption(true, 6);
    }
    
    if (m_OkButton.GetButtonState() == UBS_Down) {
		switch (m_CurrentOption) {
		case 0:
			SetState(Timers);
			break;
		case 1:
			SetState(Display);
			break;
		case 2:
			SetState(Training);
			break;
		case 3:
			SetState(Setup);
			break;
		case 4:
			//SetState(GameScreen);
			break;
		case 5:
			SetState(GameScreen);
			break;
        }
		
		return;
    }
}