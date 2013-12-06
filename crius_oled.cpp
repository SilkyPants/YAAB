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

#include <string.h> // for memset
#include <avr/pgmspace.h>

#include "crius_oled.h"
#include "i2c.h"

// I²C Address
#define OLED_Address  0x3c

extern unsigned char SmallFont[];
extern PROGMEM const unsigned char Borders[];

// default constructor
crius_oled::crius_oled()
{
} //crius_oled

// default destructor
crius_oled::~crius_oled()
{
} //~crius_oled

//
//
//
void crius_oled::initDisplay()
{
	i2c_OLED_send_cmd(OLED_Address, 0xae);    //display off
	i2c_OLED_send_cmd(OLED_Address, 0x2e);    //deactivate scrolling
	i2c_OLED_send_cmd(OLED_Address, 0xa4);    //SET All pixels OFF
	
	i2c_OLED_send_cmd(OLED_Address, 0x20);    //Set Memory Addressing Mode
	//i2c_OLED_send_cmd(OLED_Address, 0x02);    //Set Memory Addressing Mode to Page addressing mode(RESET)
	//i2c_OLED_send_cmd(OLED_Address, 0x00);    //Set Memory Addressing Mode to Horizontal addressing mode
	i2c_OLED_send_cmd(OLED_Address, 0x01);    //Set Memory Addressing Mode to Vertical addressing mode
	
	//i2c_OLED_send_cmd(OLED_Address, 0xa0);	//colum address 0 mapped to SEG0 (POR)*** wires at bottom
	i2c_OLED_send_cmd(OLED_Address, 0xa1);    //column address 127 mapped to SEG0 (POR) ** wires at top of board
	//i2c_OLED_send_cmd(OLED_Address, 0xC0);	// Scan from Right to Left (POR)         *** wires at bottom
	i2c_OLED_send_cmd(OLED_Address, 0xC8);    // Scan from Left to Right               ** wires at top
	i2c_OLED_send_cmd(OLED_Address, 0xa6);    // Set WHITE chars on BLACK background

	//i2c_OLED_send_cmd(OLED_Address, 0xa7);	// Set BLACK chars on WHITE background
	i2c_OLED_send_cmd(OLED_Address, 0x81);    // 81 Setup CONTRAST CONTROL, following byte is the contrast Value
	i2c_OLED_send_cmd(OLED_Address, 0xFF);    // af contrast value between 1 ( == dull) to 256 ( == bright)

	clear();

	i2c_OLED_send_cmd(OLED_Address, 0xaf);	//display on
}

//
//
//
void crius_oled::drawPixel( uint16_t x, uint16_t y, bool drawPixel )
{
	int bitIdx = y + ( x * LCD_HEIGHT );
	int pageIdx = bitIdx / 8;
	char pixelIdx = bitIdx % 8;
	
	if( drawPixel )
		buffer[ pageIdx ] |= 1 << pixelIdx;
	else
		buffer[ pageIdx ] &= ~( 1 << pixelIdx );
}

//
//
//
void crius_oled::drawChar( uint16_t x, uint16_t y, uint8_t character )
{
	uint8_t startChar = SmallFont[2];
	uint8_t charWidth = SmallFont[5];
	uint16_t charIdx = ( ( character - startChar ) * charWidth ) + 6;
	
	for( uint8_t bytes = 0; bytes < charWidth; bytes++ )
	{
		for( uint8_t bits = 0; bits < 8; bits++ )
		{
			bool bitValue = SmallFont[ charIdx + bytes ] & ( 1 << bits );
			
			drawPixel( x + bytes, y + bits, bitValue );
		}
	}
}

//
//
//
void crius_oled::drawString( uint16_t x, uint16_t y, uint8_t * string )
{
	uint8_t charWidth = SmallFont[5];
	uint8_t charHeight = SmallFont[4];
	uint16_t runningX = x;
	
	while( *string )
	{		
		if( *string == '\n' )
		{
			y += charHeight;
			runningX = x;
		}
		else
		{
			drawChar( runningX, y, *string );
			
			runningX += charWidth;
		}
		
		*string = *string + 1;
	}
}

//
//
//
void crius_oled::display()
{
	for( uint16_t col_page = 0; col_page < LCD_BUFFER_SIZE; col_page++ )
	{
		i2c_OLED_send_byte(OLED_Address, buffer[ col_page ] );
	}
}

//
//
//
void crius_oled::clear()
{
	memset( buffer, 0x00, LCD_BUFFER_SIZE );
	
	memcpy_P( buffer, Borders, LCD_BUFFER_SIZE );
}
