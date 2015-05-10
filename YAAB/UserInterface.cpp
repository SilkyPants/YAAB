//
//  UserInterface.cpp
//  YAAB
//
//  Created by Daniel Silk on 4/05/2015.
//  Copyright (c) 2015 Silly Kat. All rights reserved.
//

#include <string.h>

#include "UserInterface.h"

#include "pins.h"

#include "PinState.h"
#include "BreechEyesTask.h"
#include "settings.h"	

#include "strings.h"

extern BreechEyesTask eyeCycleTask;
extern const MarkerProfile g_CurrentMode;

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

void UserInterface::Init()
{
	m_Display.Init();
    
    battLevel = 100;
    eyeAnimIdx = EYE_BALL_ANIM_START;
	
	SetHeaderText(&(ModeHeaderStrings[g_CurrentMode.profileNameIndex]));
}

void UserInterface::OnSecond()
{
	// TODO: Get Battery Level
    if( battLevel <= 0 )
    {
        battLevel = 100;
    }
    else
    {
        battLevel -= 25;
    }
    
    DrawBatteryLevel( battLevel );
    
    if( eyeAnimIdx > EYE_BALL_ANIM_END )
    {
        eyeAnimIdx = EYE_BALL_ANIM_START;
    }
    
	m_Display.DrawChar(5, 4, char(eyeAnimIdx++));
}

void UserInterface::Update()
{
    char buffer[16];
	
	long eye = eyeCycleTask.GetCurrentEye();
	eye *= 100;
	eye /= 1024;
    
    sprintf(buffer, "%03ld", eye);
    
	m_Display.DrawString(3, 17, buffer, BPS_FONT);
	
	sprintf(buffer, "CURR EYE: %04u", eyeCycleTask.GetCurrentEye());

	m_Display.DrawString(3, 39, buffer);
    
    Draw();
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