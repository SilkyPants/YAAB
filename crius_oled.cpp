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
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "common.h"
#include <util/delay.h>
#include "pins.h"

#include "font.h"
#include "i2c.h"
#include "crius_oled.h"

#define FONT SmallFont

#define OLED_address  0x3c

#define SSD1306_SETCONTRAST		0x81
#define SSD1306_CONTRASTDULL	0x01
#define SSD1306_CONTRASTBRIGHT	0xFF

#define SSD1306_REMAPSEG0TO0	0xA0
#define SSD1306_REMAPSEG0TO127	0xA1

#define SSD1306_NORMALCOMSCAN	0xC0
#define SSD1306_REVERSECOMSCAN	0xC8

#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_VERTICAL_ADDRESS_MODE 0x01

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

unsigned char buffer[ LCD_BUFFER_SIZE ] =
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xFA,0x2F,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0xFA,0x2F,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0xE2,0x23,0x00,0x00,0x00,0x00,0x00,0x40,0x32,0x26,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0xFA,0x2F,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


void sendcommand(unsigned char com)
{
	i2c_OLED_send_cmd( OLED_address, com);
}

//==========================================================//
void senddata(unsigned char data)
{
	i2c_OLED_send_byte( OLED_address, data);
}

void displayBuffer()
{	
	for(int idx = 0; idx < LCD_BUFFER_SIZE; idx++)
	{
		senddata( buffer[ idx ] );
	}
}

void drawPixel( int x, int y, bool draw )
{
	int bitIdx = y + ( x * 64 );
	int pageIdx = bitIdx / 8;
	uint8_t pixelIdx = bitIdx % 8;
	
	if( draw )
		buffer[ pageIdx ] |= 1 << pixelIdx;
	else
		buffer[ pageIdx ] &= ~( 1 << pixelIdx );
}

const unsigned char * GetFont( FontType font )
{
	switch( font )
	{
		case BPS_FONT:
			return BPSFont;
			
		case LARGE_FONT:
			return LargeFont;
		
		case SMALL_FONT:
		default:
			return SmallFont;
	}
}

void drawChar( int x, int y, unsigned char character, const unsigned char * font )
{	
	uint8_t startChar = pgm_read_byte_near(font + 2);
	uint8_t charWidth = pgm_read_byte_near(font + 5);
	uint8_t charHeight = pgm_read_byte_near(font + 4);
	
	uint8_t bytesPerCol = ( charHeight / 8 ) + 1;
	int16_t charIdx = ( ( character - startChar ) * ( charWidth * bytesPerCol ) ) + 6;
	
	for( uint8_t bytes = 0; bytes < charWidth; bytes++ )
	{
		for( uint8_t cols = 0; cols < bytesPerCol; cols++ )
		{
			uint8_t maxBits = charHeight - ( cols * 8 );
			uint16_t offset = charIdx + cols + ( bytes * bytesPerCol);
			uint8_t charByte = pgm_read_byte_near( font + offset );
			
			if( maxBits > 8 )
				maxBits = 8;
			
			for( uint8_t bits = 0; bits < maxBits; bits++ )
			{
				bool bitValue = charByte & ( 1 << bits );
			
				drawPixel( x + bytes, y + ( cols * 8 ) + bits, bitValue );
			}
		}
	}
}

void drawString( int x, int y, unsigned char * string, FontType fontType )
{
	const unsigned char * font = GetFont( fontType );
	
	uint8_t charWidth = pgm_read_byte_near(font + 5);
	uint8_t charHeight = pgm_read_byte_near(font + 4);
	int runningX = x;
	
	while(*string)
	{
		if( *string == '\n' )
		{
			y += charHeight;
			runningX = x;
		}
		else if( *string == ' ' )
		{
			runningX += charWidth;
		}
		else
		{
			drawChar( runningX, y, *string, font );
			
			runningX += charWidth;
		}
		
		*string++;
	}
}

void swap(int16_t &r, int16_t &s)
{
	int16_t pSwap = r;
	r = s;
	s = pSwap;
	return;
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) 
	{
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) 
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) 
	{
		ystep = 1;
		} 
		else
		{
		ystep = -1;
	}

	for (; x0<=x1; x0++) 
	{
		if (steep) 
		{
			drawPixel(y0, x0, true);
			
	}
	else
	{
			drawPixel(x0, y0, true);
		}
		err -= dy;
		if (err < 0) 
		{
			y0 += ystep;
			err += dx;
		}
	}
}

void fillRect( int x, int y, int w, int h, bool fill )
{
	for( int i = x; i < x + w; i++ )
	{
		for( int j = y; j < y + h; j++ )
		{
			drawPixel( i, j, fill );
		}	
	}
}

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

//==========================================================//
void clear_display(void)
{
	for(int i = 0; i < LCD_BUFFER_SIZE; i++)    
	{
		senddata(0);
	}
}

//==========================================================//
void setXY(unsigned char row,unsigned char col)
{
	sendcommand(0xb0+row);                //set page address
	sendcommand(0x00+(8*col&0x0f));       //set low col address
	sendcommand(0x10+((8*col>>4)&0x0f));  //set high col address
}

//==========================================================//
void init_OLED(void)
{
	set_output(CYCLE_PORT_REG, 4);
	_delay_ms(20);
	output_high(CYCLE_PORT, 4);

	_delay_ms(50);
	
	sendcommand( SSD1306_DISPLAYOFF );
	sendcommand( SSD1306_DEACTIVATE_SCROLL );
	sendcommand( SSD1306_DISPLAYALLON_RESUME );
	
	_delay_ms(50);
	
	sendcommand( SSD1306_REMAPSEG0TO127 );
	sendcommand( SSD1306_REVERSECOMSCAN );
	
	sendcommand( SSD1306_NORMALDISPLAY );
	
	sendcommand( SSD1306_MEMORYMODE );
	sendcommand( SSD1306_VERTICAL_ADDRESS_MODE );
	
	sendcommand( SSD1306_SETCONTRAST );
	sendcommand( SSD1306_CONTRASTDULL );
	
	_delay_ms(20);
	
	setXY(0,0);
	clear_display();

	sendcommand( SSD1306_DISPLAYON );          //display on
	
	_delay_ms(20);
}

//
//
//#include <string.h> // for memset
//#include <stdlib.h>
//#include <avr/pgmspace.h>
//
//#include "common.h"
//#include <util/delay.h>
//#include "pins.h"
//
//#include "crius_oled.h"
//#include "i2c.h"
//
//// Iï¿½C Address
//#define OLED_Address  0x3c
//
//extern unsigned char SmallFont[];
////extern PROGMEM const unsigned char Borders[];
//
//uint8_t buffer[ LCD_BUFFER_SIZE ]= {
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,
//0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x40,
//0xFA,0xBF,0x00,0x00,0x00,0x00,0x00,0x40,
//0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x40,
//0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x40,
//0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,
//0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,
//0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,
//0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,
//0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,
//0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x40,
//0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x40,
//0xFA,0xBF,0x00,0x00,0x00,0x00,0x00,0x40,
//0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,
//0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,
//0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,
//0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x80,0x00,0x00,0x00,0x00,0x00,0x40,0xC2,0x87,0x00,0x00,0x00,0x00,0x00,0x40,0xF2,0x9F,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x40,0xEA,0xAF,0x00,0x00,0x00,0x00,0x00,0x40,0xEA,0xAF,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x40,0xEA,0xAF,0x00,0x00,0x00,0x00,0x00,0x40,0xEA,0xAF,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x40,0xEA,0xAF,0x00,0x00,0x00,0x00,0x00,0x40,0xEA,0xAF,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x40,0xEA,0xAF,0x00,0x00,0x00,0x00,0x00,0x40,0xEA,0xAF,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x40,0xEA,0xAF,0x00,0x00,0x00,0x00,0x00,0x40,0xEA,0xAF,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0xA0,0x00,0x00,0x00,0x00,0x00,0x40,0xFA,0xBF,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
//};
//
//// default constructor
//crius_oled::crius_oled()
//{
//} //crius_oled
//
//// default destructor
//crius_oled::~crius_oled()
//{
//} //~crius_oled
//
//void crius_oled::ssd1306_command(uint8_t cmd)
//{
//i2c_OLED_send_cmd(OLED_Address, cmd );
//}
//
////
////
////
//void crius_oled::initDisplay()
//{
//i2c_init();
//
//set_output(CYCLE_PORT_REG, 4);
//
//_delay_ms( 1000 );
//
//output_high(CYCLE_PORT, 4);
///*
//ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
//ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
//ssd1306_command(0x80);                                  // the suggested ratio 0x80
//ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
//ssd1306_command(0x3F);
//ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
//ssd1306_command(0x0);                                   // no offset
//ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
//ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
//ssd1306_command(SSD1306_VERTICAL_ADDRESS_MODE);                    // 0x8D
////if (vccstate == SSD1306_EXTERNALVCC)
//{ ssd1306_command(0x10); }
////else
////  { ssd1306_command(0x14); }
//ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
//ssd1306_command(0x00);                                  // 0x0 act like ks0108
//ssd1306_command(SSD1306_SEGREMAP | 0x1);
//ssd1306_command(SSD1306_COMSCANDEC);
//ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
//ssd1306_command(0x12);
//ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
////if (vccstate == SSD1306_EXTERNALVCC)
//{ ssd1306_command(0x9F); }
////else
////  { ssd1306_command(0xCF); }
//ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
////if (vccstate == SSD1306_EXTERNALVCC)
//{ ssd1306_command(0x22); }
//// else
////  { ssd1306_command(0xF1); }
//ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
//ssd1306_command(0x40);
//ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
//ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6
//*/
//i2c_OLED_send_cmd(OLED_Address, 0xae);    //display off
//i2c_OLED_send_cmd(OLED_Address, 0x2e);    //deactivate scrolling
//i2c_OLED_send_cmd(OLED_Address, 0xa4);    //SET All pixels OFF
//
//i2c_OLED_send_cmd(OLED_Address, 0x20);    //Set Memory Addressing Mode
//i2c_OLED_send_cmd(OLED_Address, 0x02);    //Set Memory Addressing Mode to Page addressing mode(RESET)
////i2c_OLED_send_cmd(OLED_Address, 0x00);    //Set Memory Addressing Mode to Horizontal addressing mode
//
////i2c_OLED_send_cmd(OLED_Address, 0xa0);	//colum address 0 mapped to SEG0 (POR)*** wires at bottom
////i2c_OLED_send_cmd(OLED_Address, 0xC0);	// Scan from Right to Left (POR)         *** wires at bottom
//i2c_OLED_send_cmd(OLED_Address, 0xa1);    //column address 127 mapped to SEG0 (POR) ** wires at top of board
//i2c_OLED_send_cmd(OLED_Address, 0xC8);    // Scan from Left to Right               ** wires at top
//i2c_OLED_send_cmd(OLED_Address, 0xa6);    // Set WHITE chars on BLACK background
//
//i2c_OLED_send_cmd(OLED_Address, 0x00);                //set page address
//i2c_OLED_send_cmd(OLED_Address, 0x00);       //set low col address
//i2c_OLED_send_cmd(OLED_Address, 0x10);  //set high col address
//
//i2c_OLED_send_cmd(OLED_Address, 0x20);    //Set Memory Addressing Mode
//i2c_OLED_send_cmd(OLED_Address, 0x01);    //Set Memory Addressing Mode to Vertical addressing mode
//
///*	i2c_OLED_send_cmd(OLED_Address, 0x21);
//i2c_OLED_send_cmd(OLED_Address, 0x00);
//i2c_OLED_send_cmd(OLED_Address, 0x7F);
//
//i2c_OLED_send_cmd(OLED_Address, 0x22);
//i2c_OLED_send_cmd(OLED_Address, 0x00);
//i2c_OLED_send_cmd(OLED_Address, 0x07);
//
//i2c_OLED_send_cmd(OLED_Address, 0xD3);
//i2c_OLED_send_cmd(OLED_Address, 0x0F);*/
//
////i2c_OLED_send_cmd(OLED_Address, 0xa7);	// Set BLACK chars on WHITE background
//i2c_OLED_send_cmd(OLED_Address, 0x81);    // 81 Setup CONTRAST CONTROL, following byte is the contrast Value
//i2c_OLED_send_cmd(OLED_Address, 0xFF);    // af contrast value between 1 ( == dull) to 256 ( == bright)
//
//i2c_OLED_send_cmd(OLED_Address, 0xaf);	//display on
//
//
//clear();
////drawString( 52, 3, fill_string1 );
//}
//
////
////
////
//void crius_oled::drawPixel( uint16_t x, uint16_t y, bool drawPixel )
//{
//int bitIdx = y + ( x * LCD_HEIGHT );
//int pageIdx = bitIdx / 8;
//char pixelIdx = bitIdx % 8;
//
//if( drawPixel )
//buffer[ pageIdx ] |= 1 << pixelIdx;
//else
//buffer[ pageIdx ] &= ~( 1 << pixelIdx );
//}
//
////
////
////
//void crius_oled::drawChar( uint16_t x, uint16_t y, uint8_t character )
//{
//uint8_t startChar = (SmallFont[2]);
//uint8_t charWidth = (SmallFont[5]);
//uint8_t charHeight = (SmallFont[4]);
//
//uint16_t charIdx = ( ( character - startChar ) * charWidth ) + 6;
//
//for( uint8_t bytes = 0; bytes < 8; bytes++ )
//{
//for( uint8_t bits = 0; bits < charHeight; bits++ )
//{
//bool bitValue = (SmallFont[charIdx + bytes]) & ( 1 << bits );
//
//drawPixel( x + bytes, y + bits, bitValue );
//}
//}
//}
//
////
////
////
//void crius_oled::drawString( uint16_t x, uint16_t y, uint8_t * string )
//{
//
//uint8_t charWidth = (SmallFont[5]);
//uint8_t charHeight = (SmallFont[4]);
//uint16_t runningX = x;
//
//while( *string )
//{
//if( *string == '\n' )
//{
//y += charHeight;
//runningX = x;
//}
//else
//{
//drawChar( runningX, y, *string );
//
//runningX += charWidth;
//}
//
//*string = *string + 1;
//}
//}
//
////
////
////
//void crius_oled::display()
//{
///*for(int col = 0; col < 128; col++)     // show 128* 64 picture
//{
//for( int row = 0; row < 8; row++ )
//{
//int idx = ( col * 8 ) + row;
//i2c_OLED_send_byte( OLED_Address, buffer[idx] );
//}
//}*/
//
//for( uint16_t col_page = 0; col_page < LCD_BUFFER_SIZE; col_page++ )
//{
//i2c_OLED_send_byte(OLED_Address, buffer[ col_page ] );
//}
//}
//
////
////
////
//void crius_oled::clear()
//{
////memset( buffer, 0x00, LCD_BUFFER_SIZE );
//
////memcpy_P( buffer, Borders, LCD_BUFFER_SIZE );
//
////for(uint16_t col = 0; col < 128; col++)     // show 128* 64 picture
///*{
//for( uint16_t row = 1; row < 63; row++ )
//{
//drawPixel( 1, row, 1 );//col % 2 == 0 );
//}
//}*/
//}
//
//