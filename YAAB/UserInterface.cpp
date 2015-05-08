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

#include "crius_oled.h"

#include "PinState.h"
#include "BreechEyesTask.h"
#include "settings.h"	

#include "strings.h"

extern BreechEyesTask eyeCycleTask;
extern const MarkerProfile g_CurrentMode;

static void UpPressed();
static void SelectPressed();
static void DownPressed();

PinState upPressed(UpPressed, &INPUT_PIN_REG, UP_BUTTON_PIN, true);
PinState selectPressed(SelectPressed, &INPUT_PIN_REG, OK_BUTTON_PIN, true);
PinState downPressed(DownPressed, &INPUT_PIN_REG, DN_BUTTON_PIN, true);

void drawBatteryLevel( uint8_t battPercent );

CRIUS_OLED g_Display;

///
/// UI functions
///

void UI_SetHeaderText(const char* const* string)
{
	char buffer[17];
	
	strcpy_P(buffer, (PGM_P)pgm_read_word(string));
	uint8_t x = 64 - (strlen(buffer) * 3);
	
	g_Display.DrawString(x, 4, buffer);
}

void UI_Init()
{
	g_Display.Init();
	
	UI_SetHeaderText(&(ModeHeaderStrings[g_CurrentMode.profileNameIndex]));
}

void UI_SecondTick()
{
	// TODO: Get Battery Level
    static uint8_t battLevel = 100;
    
    if( battLevel <= 0 )
    {
        battLevel = 100;
    }
    else
    {
        battLevel -= 25;
    }
    
    drawBatteryLevel( battLevel );
    
    static uint8_t eyeAnimIdx = EYE_BALL_ANIM_START;
    
    if( eyeAnimIdx > EYE_BALL_ANIM_END )
    {
        eyeAnimIdx = EYE_BALL_ANIM_START;
    }
    
	g_Display.DrawChar(5, 4, char(eyeAnimIdx++));
}

void UI_Update()
{
    upPressed.Update();
    selectPressed.Update();
    downPressed.Update();
    
    char buffer[16];
	
	long eye = eyeCycleTask.GetCurrentEye();
	eye *= 100;
	eye /= 1024;
    
    sprintf(buffer, "%03ld", eye);
    
	g_Display.DrawString(3, 17, buffer, BPS_FONT);
	
	sprintf(buffer, "CURR EYE: %04u", eyeCycleTask.GetCurrentEye());

	g_Display.DrawString(3, 39, buffer);
}

void UI_Draw()
{
    // Display the LCD
	g_Display.DisplayBuffer();
}

///
/// Drawing Functions

void drawBatteryLevel( uint8_t battPercent )
{
	g_Display.FillRect(112, 5, 11, 5, false);

	if (battPercent >= 25)
	{
		g_Display.DrawLine(121, 5, 121, 9);
		g_Display.DrawLine(122, 5, 122, 9);
	}

	if (battPercent >= 50)
	{
		g_Display.DrawLine(118, 5, 118, 9);
		g_Display.DrawLine(119, 5, 119, 9);
	}

	if (battPercent >= 75)
	{
		g_Display.DrawLine(115, 5, 115, 9);
		g_Display.DrawLine(116, 5, 116, 9);
	}

	if (battPercent >= 100)
	{
		g_Display.DrawLine(112, 5, 112, 9);
		g_Display.DrawLine(113, 5, 113, 9);
	}
}

///
/// Input functions
///

static void UpPressed()
{
    
}

static void SelectPressed()
{
    
}

static void DownPressed()
{
    
}