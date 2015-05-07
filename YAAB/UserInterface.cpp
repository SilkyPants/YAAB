//
//  UserInterface.cpp
//  YAAB
//
//  Created by Daniel Silk on 4/05/2015.
//  Copyright (c) 2015 Silly Kat. All rights reserved.
//

#include "UserInterface.h"

#include "pins.h"

#include "crius_oled.h"

#include "PinState.h"
#include "BreechEyesTask.h"

extern BreechEyesTask eyeCycleTask;

static void UpPressed();
static void SelectPressed();
static void DownPressed();

PinState upPressed(UpPressed, &INPUT_PIN_REG, UP_BUTTON_PIN, true);
PinState selectPressed(SelectPressed, &INPUT_PIN_REG, OK_BUTTON_PIN, true);
PinState downPressed(DownPressed, &INPUT_PIN_REG, DN_BUTTON_PIN, true);


unsigned char headerString[]    =   "SEMI";
unsigned char eyeStateString[]  =   "$&'(";
unsigned char menuOptions[]     =   "@ SELECT FIRE MODE\n  SELECT BPS\n  TRAINING MODE\n  SHOW STATISTICS\n  SET GAME TIMER\n  SET GAME ALARM";

void drawBatteryLevel( uint8_t battPercent );

CRIUS_OLED g_Display;

///
/// UI functions
///

void UI_Init()
{
	g_Display.Init();
	g_Display.DrawString(52, 4, headerString);
}

void UI_SecondTick()
{
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
    
    static uint8_t eyeAnimIdx = 0;
    
    if( eyeAnimIdx >= 3 )
    {
        eyeAnimIdx = 0;
    }
    else
    {
        eyeAnimIdx++;
    }
    
	g_Display.DrawChar(5, 4, eyeStateString[eyeAnimIdx]);
}

void UI_Update()
{
    upPressed.Update();
    selectPressed.Update();
    downPressed.Update();
    
    char buffer[16];
    
    sprintf(buffer, "CURR EYE: %u", eyeCycleTask.GetCurrentEye());
    
	g_Display.DrawString(3, 17, buffer);
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