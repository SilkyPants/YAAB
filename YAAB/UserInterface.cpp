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

///
/// UI functions
///

void UI_Init()
{
    init_OLED();
    
    drawBatteryLevel( 100 );
    
    drawString( 52,  4, headerString );
    drawString(  5,  4, eyeStateString );
    drawString(  3, 17, menuOptions );
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
    
    drawChar(  5,  4, eyeStateString[eyeAnimIdx] );
    
    unsigned char buffer[16];
    
    sprintf((char*)buffer, "CURR EYE: %u", eyeCycleTask.GetCurrentEye());
    
    drawString(  3, 17, buffer );
}

void UI_Update()
{
    upPressed.Update();
    selectPressed.Update();
    downPressed.Update();
}

void UI_Draw()
{
    // Display the LCD
    displayBuffer();
}

///
/// Drawing Functions

void drawBatteryLevel( uint8_t battPercent )
{
    fillRect( 112, 5, 11, 5, false );
    
    if( battPercent >= 25 )
    {
        drawLine( 121, 5, 121, 9 );
        drawLine( 122, 5, 122, 9 );
    }
    
    if( battPercent >= 50 )
    {
        drawLine( 118, 5, 118, 9 );
        drawLine( 119, 5, 119, 9 );
    }
    
    if( battPercent >= 75 )
    {
        drawLine( 115, 5, 115, 9 );
        drawLine( 116, 5, 116, 9 );
    }
    
    if( battPercent >= 100 )
    {
        drawLine( 112, 5, 112, 9 );
        drawLine( 113, 5, 113, 9 );
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