/*
YAAB - Yet Another Autococker Board
Arduino based autococker board developed around the platform and ATMEL AVR
chips

Copyright (C) 2012  Dan Silk

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

#include "Arduino.h"

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define LCD_BUFFER_SIZE ( ( LCD_WIDTH * LCD_HEIGHT ) / 8 ) 

class crius_oled
{
//variables
public:
protected:
private:

//functions
public:
	crius_oled();
	~crius_oled();
	
	void initDisplay();
	
	void drawPixel( uint16_t x, uint16_t y, bool drawPixel );
	void drawChar( uint16_t x, uint16_t y, uint8_t character);
	void drawString( uint16_t x, uint16_t y, uint8_t * string);
	void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
	
	void display();
	void clear();
	
protected:
private:
  void ssd1306_command(uint8_t cmd);


}; //crius_oled

#endif //__CRIUS_OLED_H__

