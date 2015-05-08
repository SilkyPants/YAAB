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
PROGMEM const char strFireMode_Semi[]		= "SEMI";
PROGMEM const char strFireMode_Pump[] 		= "PUMP";
PROGMEM const char strFireMode_Auto[] 		= "AUTO";
PROGMEM const char strFireMode_Burst[] 		= "BURST";
PROGMEM const char strFireMode_Reactive[]	= "REACTIVE";

const char * const ModeHeaderStrings[] PROGMEM = {
	strFireMode_Semi,
	strFireMode_Pump,
	strFireMode_Auto,
	strFireMode_Burst,
	strFireMode_Reactive
};

PROGMEM const char strTraining_Semi[]		= "TRAINING [SEMI]";
PROGMEM const char strTraining_Pump[]		= "TRAINING [PUMP]";
PROGMEM const char strTraining_Auto[]		= "TRAINING [AUTO]";
PROGMEM const char strTraining_Burst[]		= "TRAINING [BRST]";
PROGMEM const char strTraining_Reactive[]	= "TRAINING [RCTV]";

const char * const TrainingHeaderStrings[] PROGMEM = {
	strTraining_Semi,
	strTraining_Pump,
	strTraining_Auto,
	strTraining_Burst,
	strTraining_Reactive
};

///
/// Common
PROGMEM const char strMenu_On[]				= "ON";
PROGMEM const char strMenu_Off[]			= "OFF";
PROGMEM const char strMenu_Trigger[]		= "TRIGGER";
PROGMEM const char strMenu_Button[]			= "BUTTON";
PROGMEM const char strMenu_Enabled[]		= "ENABLED";
PROGMEM const char strMenu_Disabled[]		= "DISABLED";

///
/// Main Menu
PROGMEM const char strMenu_Menu[]			= "MENU";
PROGMEM const char strMenu_Sleep[]			= "TURN OFF";
PROGMEM const char strMenu_FireModes[]		= "PROFILES";
PROGMEM const char strMenu_Timers[]			= "TIMER";
PROGMEM const char strMenu_Display[]		= "DISPLAY";
PROGMEM const char strMenu_Training[]		= "TRAINING";
PROGMEM const char strMenu_Setup[]			= "SETUP";

///
/// Timer
PROGMEM const char strMenu_GameTimer[]		= "GAME TIMER";
PROGMEM const char strMenu_AlarmTimer[]		= "ALARM";
PROGMEM const char strMenu_StartOn[]		= "START ON";

///
/// Display
PROGMEM const char strMenu_Stats[]			= "STATISTICS";
PROGMEM const char strMenu_Graphic[]		= "GRAPHIC";

///
/// Training
PROGMEM const char strMenu_TrainingMode[]	= "TRAINING MODE"; // Rename to something else?

///
/// Setup
PROGMEM const char strMenu_Eyes[]			= "EYES";
PROGMEM const char strMenu_Timing[]			= "TIMING";
PROGMEM const char strMenu_Modes[]			= "MODES";
PROGMEM const char strMenu_Power[]			= "POWER";

///
///
PROGMEM const char strMenu_XMinutes[]		= "%d MINUTES";
PROGMEM const char strMenu_TimerEnd[]		= "TIMER END";
PROGMEM const char strMenu_Never[]			= "NEVER";

///
/// Eyes
PROGMEM const char strMenu_BallValue[]		= "BALL VALUE";
PROGMEM const char strMenu_BallTime[]		= "BALL TIME";
PROGMEM const char strMenu_EmptyTime[]		= "EMPTY TIME";
PROGMEM const char strMenu_TimeOut[]		= "TIMEOUT";

///
/// Timings
PROGMEM const char strMenu_SearOn[]			= "SEAR ON";
PROGMEM const char strMenu_PneuDelay[]		= "PNEU DELAY";
PROGMEM const char strMenu_PneuOn[]			= "PNEU ON";
PROGMEM const char strMenu_PneuOff[]		= "PNEU OFF";

///
/// String Enums
enum MenuStringsEnum
{
	MENU_ON,
	MENU_OFF,
	MENU_TRIGGER,
	MENU_BUTTON,
	MENU_ENABLED,
	MENU_DISABLED,
	MENU_MENU,
	MENU_SLEEP,
	MENU_FIREMODES,
	MENU_TIMERS,
	MENU_DISPLAY,
	MENU_TRAINING,
	MENU_SETUP,
	MENU_GAMETIMER,
	MENU_ALARMTIMER,
	MENU_STARTON,
	MENU_STATS,
	MENU_GRAPHIC,
	MENU_TRAININGMODE,
	MENU_EYES,
	MENU_TIMING,
	MENU_MODES,
	MENU_POWER,
	MENU_XMINUTES,
	MENU_TIMEREND,
	MENU_NEVER,
	MENU_BALLVALUE,
	MENU_BALLTIME,
	MENU_EMPTYTIME,
	MENU_TIMEOUT,
	MENU_SEARON,
	MENU_PNEUDELAY,
	MENU_PNEUON,
	MENU_PNEUOFF
};

///
/// Main string table
const char * const MenuStrings[] PROGMEM = {
	strMenu_On,
	strMenu_Off,
	strMenu_Trigger,
	strMenu_Button,
	strMenu_Enabled,
	strMenu_Disabled,
	strMenu_Menu,
	strMenu_Sleep,
	strMenu_FireModes,
	strMenu_Timers,
	strMenu_Display,
	strMenu_Training,
	strMenu_Setup,
	strMenu_GameTimer,
	strMenu_AlarmTimer,
	strMenu_StartOn,
	strMenu_Stats,
	strMenu_Graphic,
	strMenu_TrainingMode,
	strMenu_Eyes,
	strMenu_Timing,
	strMenu_Modes,
	strMenu_Power,
	strMenu_XMinutes,
	strMenu_TimerEnd,
	strMenu_Never,
	strMenu_BallValue,
	strMenu_BallTime,
	strMenu_EmptyTime,
	strMenu_TimeOut,
	strMenu_SearOn,
	strMenu_PneuDelay,
	strMenu_PneuOn,
	strMenu_PneuOff
};

#endif /* STRINGS_H_ */