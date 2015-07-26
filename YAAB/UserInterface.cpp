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

#include <string.h>

#include "UserInterface.h"

#include "PinState.h"

#define MENU_OPTIONS_OFFSET(option) 16 + (7 * option)

///
/// UI functions
///

void UserInterface::SetHeaderText(const char* const* string)
{
	char buffer[17];
	
	strcpy_P(buffer, (PGM_P)pgm_read_word(string));
	uint8_t x = 64 - (strlen(buffer) * 3);
	
	m_Display.DrawString(x, 4, buffer);
}

void UserInterface::CreateOption(const char *const *string)
{
    DrawString_P(11, MENU_OPTIONS_OFFSET(m_NumOptions++), string);
}

void UserInterface::DrawString_P(uint8_t x, uint8_t y, const char* const* string)
{
    char buffer[23];
    
    strcpy_P(buffer, (PGM_P)pgm_read_word(string));
    
    m_Display.DrawString(x, y, buffer);
}

void UserInterface::Init()
{
	m_Display.Init();
    
    m_UpButton.Init(&INPUT_PIN_REG, UP_BUTTON_PIN);
    m_OkButton.Init(&INPUT_PIN_REG, OK_BUTTON_PIN);
    m_DnButton.Init(&INPUT_PIN_REG, DN_BUTTON_PIN);
    
    battLevel = 100;
    eyeAnimIdx = EYE_BALL_ANIM_START;
    
    m_CurrentOption = m_NumOptions = 0;
    m_CurrentDepth = 0;
    m_States[m_CurrentDepth] = GameScreen;
    
    EnterState();
}

void UserInterface::PushState(MenuStates p_NewState)
{
    // If we are moving from the Game screen, clear everything
    if (CurrentState() == GameScreen) {
        
        m_Display.ClearDisplay();
    }
    
    m_States[++m_CurrentDepth] = p_NewState;
    
    EnterState();
}

void UserInterface::PopState()
{
    m_CurrentDepth--;
    
    // If we are moving to the Game screen, clear everything
    if (CurrentState() == GameScreen && g_Settings.gameScreenType != GSM_Graphic) {
        
        m_Display.ClearDisplay();
    }
    
    EnterState();
}

void UserInterface::EnterState()
{
    // Clear display
    if (CurrentState() != GameScreen) {
        m_Display.FillRect(19, 4,  89,  7, false); // Header area
        m_Display.FillRect(3, 16, 122, 45, false); // Bottom section
    }
    
    // Clear options
    m_NumOptions = 0;
    m_CurrentOption = 0;
    
    // Add option to go back a state
    if (CurrentState() != GameScreen) {
        CreateOption(&(MenuStrings[MENU_BACK]));
        
        ShowCursor(true);
    }
    
    switch (CurrentState()) {
            CASE_ENTER_STATE(GameScreen)
            CASE_ENTER_STATE(MenuRoot)

            CASE_ENTER_STATE(Profiles)
            CASE_ENTER_STATE(Timers)
            CASE_ENTER_STATE(Display)
            CASE_ENTER_STATE(Training)
            CASE_ENTER_STATE(Setup)

            CASE_ENTER_STATE(SetGameTimer)
            CASE_ENTER_STATE(SetAlarmTimer)

            CASE_ENTER_STATE(DisplayGraphic)

            CASE_ENTER_STATE(SetupEyes)
            CASE_ENTER_STATE(SetupTimings)
            CASE_ENTER_STATE(SetupModes)
            CASE_ENTER_STATE(SetupPower)

            CASE_ENTER_STATE(EyesBallValue)
    };
}

void UserInterface::OnSecond()
{
	if (!(CurrentState() == GameScreen && g_Settings.gameScreenType == GSM_Graphic))
	{
		// TODO: Get Battery Level
		if (battLevel <= 0)
		{
			battLevel = 100;
		}
		else
		{
			battLevel -= 25;
		}

		DrawBatteryLevel(battLevel);

		if (eyeAnimIdx > EYE_BALL_ANIM_END)
		{
			eyeAnimIdx = EYE_BALL_ANIM_START;
		}

		m_Display.DrawChar(5, 4, char(eyeAnimIdx++));
	}
}

void UserInterface::Update()
{
    // Update the current state
    switch (CurrentState()) {
            CASE_UPDATE_STATE(GameScreen)
			CASE_UPDATE_STATE(MenuRoot)

			CASE_UPDATE_STATE(Profiles)
			CASE_UPDATE_STATE(Timers)
			CASE_UPDATE_STATE(Display)
			CASE_UPDATE_STATE(Training)
			CASE_UPDATE_STATE(Setup)

			CASE_UPDATE_STATE(SetGameTimer)
			CASE_UPDATE_STATE(SetAlarmTimer)

			CASE_UPDATE_STATE(DisplayGraphic)

			CASE_UPDATE_STATE(SetupEyes)
			CASE_UPDATE_STATE(SetupTimings)
			CASE_UPDATE_STATE(SetupModes)
			CASE_UPDATE_STATE(SetupPower)

			CASE_UPDATE_STATE(EyesBallValue)
    };
    
    // Common check to go back
    if (CurrentState() != GameScreen) {
        
        if (m_OkButton.IsDown() && !m_OkButton.IsHeld() && m_CurrentOption == 0) {
            
            PopState();
        }
    }
    
    Draw();
}

void UserInterface::UpdateControls()
{
    m_UpButton.UpdateState();
    m_OkButton.UpdateState();
    m_DnButton.UpdateState();
}

///
/// Cursor functions

void UserInterface::ChangeOption(bool up)
{
	// Remove last cursor position
	ShowCursor(false);

	if (up) {
		if (m_CurrentOption == 0)
			m_CurrentOption = m_NumOptions - 1;
		else
			m_CurrentOption--;
	}
	else {
		if (m_CurrentOption == m_NumOptions - 1)
			m_CurrentOption = 0;
		else
			m_CurrentOption++;
	}

	ShowCursor(true);
}

void UserInterface::ShowCursor(bool show)
{
	if (show)
		m_Display.DrawString(5, MENU_OPTIONS_OFFSET(m_CurrentOption), CURSOR_CHARACTER);
	else
		m_Display.DrawString(5, MENU_OPTIONS_OFFSET(m_CurrentOption), " ");
}

///
/// Drawing Functions

void UserInterface::Draw()
{
    // Display the LCD
	m_Display.DisplayBuffer();
}

void UserInterface::DrawBatteryLevel( uint8_t battPercent )
{
	m_Display.FillRect(112, 5, 11, 5, false);

	if (battPercent >= 25)
	{
		m_Display.DrawLine(121, 5, 121, 9);
		m_Display.DrawLine(122, 5, 122, 9);
	}

	if (battPercent >= 50)
	{
		m_Display.DrawLine(118, 5, 118, 9);
		m_Display.DrawLine(119, 5, 119, 9);
	}

	if (battPercent >= 75)
	{
		m_Display.DrawLine(115, 5, 115, 9);
		m_Display.DrawLine(116, 5, 116, 9);
	}

	if (battPercent >= 100)
	{
		m_Display.DrawLine(112, 5, 112, 9);
		m_Display.DrawLine(113, 5, 113, 9);
	}
}