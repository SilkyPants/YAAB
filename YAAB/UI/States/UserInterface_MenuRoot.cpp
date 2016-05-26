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

#include "../UserInterface.h"

void UserInterface::MenuRoot_OnEnter()
{
    SetHeaderText(&(MenuStrings[MENU_MENU]));

    CreateOption(&(MenuStrings[MENU_TIMERS]));
    CreateOption(&(MenuStrings[MENU_DISPLAY]));
    CreateOption(&(MenuStrings[MENU_TRAINING]));
    CreateOption(&(MenuStrings[MENU_SETUP]));
    CreateOption(&(MenuStrings[MENU_SLEEP]));

	ShowCursor(true);
}

void UserInterface::MenuRoot_Update()
{
	if (m_DnButton.IsDown()) {
		ChangeOption(false);
    }

    if (m_UpButton.IsDown()) {
        ChangeOption(true);
    }

    if (m_OkButton.IsDown()) {
		switch (m_CurrentOption) {
		case 1:
			PushState(Timers);
			break;
		case 2:
			PushState(Display);
			break;
		case 3:
			PushState(Training);
			break;
		case 4:
			PushState(Setup);
			break;
		case 5:
			//Sleep
			break;
        }

		return;
    }
}
