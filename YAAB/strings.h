/*
YAAB - Yet Another Autococker Board
Arduino based autococker board developed around the platform and ATMEL AVR
chips

Copyright (C) 2013  Dan Silk

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


#include <avr/pgmspace.h>

#include "common.h"

#ifndef STRINGS_H_
#define STRINGS_H_

/*
Menu Headings
*/
PROGMEM const unsigned char strFireMode_Semi[]		= "SEMI";
PROGMEM const unsigned char strFireMode_Pump[]		= "PUMP";
PROGMEM const unsigned char strFireMode_Auto[]		= "AUTO";
PROGMEM const unsigned char strFireMode_Burst[]		= "BURST";
PROGMEM const unsigned char strFireMode_Reactive[]	= "REACTIVE";

const unsigned char * ModeHeaderStrings[] PROGMEM = {
	strFireMode_Semi,
	strFireMode_Pump,
	strFireMode_Auto,
	strFireMode_Burst,
	strFireMode_Reactive
};

PROGMEM const unsigned char strTraining_Semi[]		= "TRAINING [SEMI]";
PROGMEM const unsigned char strTraining_Pump[]		= "TRAINING [PUMP]";
PROGMEM const unsigned char strTraining_Auto[]		= "TRAINING [AUTO]";
PROGMEM const unsigned char strTraining_Burst[]		= "TRAINING [BRST]";
PROGMEM const unsigned char strTraining_Reactive[]	= "TRAINING [RCTV]";

const unsigned char * TrainingHeaderStrings[] PROGMEM = {
	strTraining_Semi,
	strTraining_Pump,
	strTraining_Auto,
	strTraining_Burst,
	strTraining_Reactive
};

PROGMEM const unsigned char strMenu_Menu[]			= "MENU";
PROGMEM const unsigned char strMenu_Sleep[]			= "TURN OFF";
PROGMEM const unsigned char strMenu_RateOfFire[]	= "RATE OF FIRE";
PROGMEM const unsigned char strMenu_FireModes[]		= "PROFILES";
PROGMEM const unsigned char strMenu_Hardware[]		= "HARDWARE";
PROGMEM const unsigned char strMenu_Timing[]		= "MARKER TIMING";
PROGMEM const unsigned char strMenu_Filter[]		= "EYE SETTINGS";
PROGMEM const unsigned char strMenu_GameScreen[]	= "GAME SCREEN";
PROGMEM const unsigned char strMenu_Training[]		= "TRAINING";
PROGMEM const unsigned char strMenu_Timers[]		= "GAME TIMERS";

enum MenuStringsEnum
{
	MENU_MENU,
	MENU_SLEEP,
	MENU_RATE_OF_FIRE,
	MENU_FIRE_MODES,
	MENU_HARDWARE,
	MENU_TIMING,
	MENU_FILTER,
	MENU_GAME_SCREEN,
	MENU_TRAINING,
	MENU_TIMERS,
};

const unsigned char * MenuStrings[] PROGMEM = {
	strMenu_Menu,
	strMenu_Sleep,
	strMenu_RateOfFire,
	strMenu_FireModes,
	strMenu_Hardware,
	strMenu_Timing,
	strMenu_Filter,
	strMenu_GameScreen,
	strMenu_Training,
	strMenu_Timers
};

#endif /* STRINGS_H_ */