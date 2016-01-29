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

void UserInterface::Setup_OnEnter()
{
	SetHeaderText(&(MenuStrings[MENU_SETUP]));

	CreateOption(&(MenuStrings[MENU_EYES]));
	CreateOption(&(MenuStrings[MENU_MODES]));
	CreateOption(&(MenuStrings[MENU_POWER]));
	CreateOption(&(MenuStrings[MENU_TIMING]));

	ShowCursor(true);
}

void UserInterface::Setup_Update()
{
	if (m_DnButton.GetButtonState() == UBS_Down) {
		ChangeOption(false);
	}


	if (m_UpButton.GetButtonState() == UBS_Down) {
		ChangeOption(true);
	}

	if (m_OkButton.GetButtonState() == UBS_Down) {
		switch (m_CurrentOption) {
		case 1:
			PushState(SetupEyes);
			break;
		case 2:
			PushState(SetupModes);
			break;
		case 3:
			PushState(SetupPower);
			break;
		case 4:
			PushState(SetupTimings);
			break;
		}

		return;
	}
}
