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


#ifndef __CRIUS_OLED_H__
#define __CRIUS_OLED_H__

#include "strings.h"

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define LCD_BUFFER_SIZE ( ( LCD_WIDTH * LCD_HEIGHT ) / 8 ) 

enum FontType
{
	SMALL_FONT,
	LARGE_FONT,
	BPS_FONT
};

void fillRect( int x, int y, int w, int h, bool fill = true );
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void drawChar( int x, int y, unsigned char character, FontType type = SMALL_FONT );
void drawString( int x, int y, unsigned char * string, FontType type = SMALL_FONT );

void clear_display(void);
void init_OLED(void);
void displayBuffer();

#endif //__CRIUS_OLED_H__

