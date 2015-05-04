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

static void UpPressed();
static void SelectPressed();
static void DownPressed();

PinState upPressed(UpPressed, &INPUT_PIN_REG, UP_BUTTON_PIN, true);
PinState selectPressed(SelectPressed, &INPUT_PIN_REG, OK_BUTTON_PIN, true);
PinState downPressed(DownPressed, &INPUT_PIN_REG, DN_BUTTON_PIN, true);

unsigned char headerString[]    =   "SEMI";
unsigned char eyeStateString[]  =   "(";
unsigned char menuOptions[]     =   "@ SELECT FIRE MODE\n  SELECT BPS\n  TRAINING MODE\n  SHOW STATISTICS\n  SET GAME TIMER\n  SET GAME ALARM";

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

void UI_UpdateBattery(uint8_t p_BattLevel)
{
    drawBatteryLevel( p_BattLevel );
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