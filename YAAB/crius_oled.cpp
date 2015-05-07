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

#include <stdlib.h>
#include <avr/pgmspace.h>

#include "common.h"
#include <util/delay.h>
#include "pins.h"

#include "font.h"
#include "i2c.h"
#include "crius_oled.h"

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

//unsigned char buffer[ LCD_BUFFER_SIZE ] =
//{
//	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xFA,0x2F,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0xFA,0x2F,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x20,0x00,0x00,0x00,0x00,0x00,0x40,0xE2,0x23,0x00,0x00,0x00,0x00,0x00,0x40,0x32,0x26,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0x0A,0x28,0x00,0x00,0x00,0x00,0x00,0x40,0xFA,0x2F,0x00,0x00,0x00,0x00,0x00,0x40,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
//};

///
/// Drawing Functions

void CRIUS_OLED::FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool fill)
{
	for (int i = x; i < x + w; i++)
	{
		for (int j = y; j < y + h; j++)
		{
			DrawPixel(i, j, fill);
		}
	}
}

void CRIUS_OLED::Swap(uint8_t &r, uint8_t &s)
{
	uint8_t pSwap = r;
	r = s;
	s = pSwap;
}

void CRIUS_OLED::DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep)
	{
		Swap(x0, y0);
		Swap(x1, y1);
	}

	if (x0 > x1)
	{
		Swap(x0, x1);
		Swap(y0, y1);
	}

	uint8_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	uint8_t err = dx / 2;
	uint8_t ystep;

	if (y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for (; x0 <= x1; x0++)
	{
		if (steep)
		{
			DrawPixel(y0, x0, true);
		}
		else
		{
			DrawPixel(x0, y0, true);
		}
		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}

void CRIUS_OLED::DrawChar(uint8_t x, uint8_t y, const char character, FontType type)
{
	this->DrawCharInternal(x, y, character, GetFont(type));
}

void CRIUS_OLED::DrawString(uint8_t x, uint8_t y, const char * string, FontType type)
{
	const uint8_t * font = GetFont(type);

	uint8_t charWidth = pgm_read_byte_near(font + 5);
	uint8_t charHeight = pgm_read_byte_near(font + 4);
	int runningX = x;

	while (*string)
	{
		if (*string == '\n')
		{
			y += charHeight;
			runningX = x;
		}
		else if (*string == ' ')
		{
			runningX += charWidth;
		}
		else
		{
			DrawCharInternal(runningX, y, *string, font);

			runningX += charWidth;
		}

		string = string + 1;
	}
}

void CRIUS_OLED::DrawPixel(uint8_t x, uint8_t y, bool draw)
{
	if (x < 0 || x >= LCD_WIDTH || y < 0 || y >= LCD_HEIGHT)
		return;

	int bitIdx = y + (x * LCD_HEIGHT);
	uint8_t pageIdx = bitIdx / 8;
	uint8_t pixelIdx = bitIdx % 8;

	if (draw)
		buffer[pageIdx] |= 1 << pixelIdx;
	else
		buffer[pageIdx] &= ~(1 << pixelIdx);
}

void CRIUS_OLED::DrawCharInternal(uint8_t x, uint8_t y, const char character, const uint8_t * font)
{
	uint8_t startChar = pgm_read_byte_near(font + 2);
	uint8_t charWidth = pgm_read_byte_near(font + 5);
	uint8_t charHeight = pgm_read_byte_near(font + 4);

	uint8_t uint8_tsPerCol = (charHeight / 8) + 1;
	int16_t charIdx = ((character - startChar) * (charWidth * uint8_tsPerCol)) + 6;

	for (uint8_t uint8_ts = 0; uint8_ts < charWidth; uint8_ts++)
	{
		for (uint8_t cols = 0; cols < uint8_tsPerCol; cols++)
		{
			uint8_t maxBits = charHeight - (cols * 8);
			uint16_t offset = charIdx + cols + (uint8_ts * uint8_tsPerCol);
			uint8_t charByte = pgm_read_byte_near(font + offset);

			if (maxBits > 8)
				maxBits = 8;

			for (uint8_t bits = 0; bits < maxBits; bits++)
			{
				bool bitValue = charByte & (1 << bits);

				this->DrawPixel(x + uint8_ts, y + (cols * 8) + bits, bitValue);
			}
		}
	}
}

const uint8_t * CRIUS_OLED::GetFont(FontType font)
{
	switch (font)
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

///
/// System

void CRIUS_OLED::Init()
{

	set_output(CYCLE_PORT_REG, 4);
	_delay_ms(20);
	output_high(CYCLE_PORT, 4);

	_delay_ms(50);

	SendCommand(SSD1306_DISPLAYOFF);
	SendCommand(SSD1306_DEACTIVATE_SCROLL);
	SendCommand(SSD1306_DISPLAYALLON_RESUME);

	_delay_ms(50);

	SendCommand(SSD1306_REMAPSEG0TO127);
	SendCommand(SSD1306_REVERSECOMSCAN);

	SendCommand(SSD1306_NORMALDISPLAY);

	SendCommand(SSD1306_MEMORYMODE);
	SendCommand(SSD1306_VERTICAL_ADDRESS_MODE);

	SendCommand(SSD1306_SETCONTRAST);
	SendCommand(SSD1306_CONTRASTDULL);

	_delay_ms(20);

	SetXY(0, 0);

	ClearDisplay();

	SendCommand(SSD1306_DISPLAYON);          //display on

	_delay_ms(20);
}

void CRIUS_OLED::ClearDisplay()
{
	memcpy_P(buffer, Borders, LCD_BUFFER_SIZE);

	DisplayBuffer();
}

void CRIUS_OLED::DisplayBuffer()
{
	for (int idx = 0; idx < LCD_BUFFER_SIZE; idx++)
	{
		this->SendData(buffer[idx]);
	}
}

void CRIUS_OLED::SendCommand(uint8_t cmd)
{
	i2c_OLED_send_cmd(OLED_address, cmd);
}

void CRIUS_OLED::SendData(uint8_t data)
{
	i2c_OLED_send_byte(OLED_address, data);
}

void CRIUS_OLED::SetXY(uint8_t row, uint8_t col)
{
	SendCommand(0xb0 + row);                //set page address
	SendCommand(0x00 + (8 * col & 0x0f));       //set low col address
	SendCommand(0x10 + ((8 * col >> 4) & 0x0f));  //set high col address
}

/*
void sendcommand(unsigned char com)
{
	i2c_OLED_send_cmd( OLED_address, com);
}

//==========================================================//
void senddata(unsigned char data)
{
	i2c_OLED_send_uint8_t( OLED_address, data);
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
	if( x < 0 || x >= LCD_WIDTH || y < 0 || y >= LCD_HEIGHT )
		return;
	
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

void drawCharInternal( int x, int y, unsigned char character, const unsigned char * font )
{	
	uint8_t startChar = pgm_read_uint8_t_near(font + 2);
	uint8_t charWidth = pgm_read_uint8_t_near(font + 5);
	uint8_t charHeight = pgm_read_uint8_t_near(font + 4);
	
	uint8_t uint8_tsPerCol = ( charHeight / 8 ) + 1;
	int16_t charIdx = ( ( character - startChar ) * ( charWidth * uint8_tsPerCol ) ) + 6;
	
	for( uint8_t uint8_ts = 0; uint8_ts < charWidth; uint8_ts++ )
	{
		for( uint8_t cols = 0; cols < uint8_tsPerCol; cols++ )
		{
			uint8_t maxBits = charHeight - ( cols * 8 );
			uint16_t offset = charIdx + cols + ( uint8_ts * uint8_tsPerCol);
			uint8_t charuint8_t = pgm_read_uint8_t_near( font + offset );
			
			if( maxBits > 8 )
				maxBits = 8;
			
			for( uint8_t bits = 0; bits < maxBits; bits++ )
			{
				bool bitValue = charuint8_t & ( 1 << bits );
			
				drawPixel( x + uint8_ts, y + ( cols * 8 ) + bits, bitValue );
			}
		}
	}
}

void drawChar( int x, int y, unsigned char character, FontType fontType )
{
    drawCharInternal( x, y, character, GetFont( fontType ) );
}

void drawString( int x, int y, unsigned char * string, FontType fontType )
{
	const unsigned char * font = GetFont( fontType );
	
	uint8_t charWidth = pgm_read_uint8_t_near(font + 5);
	uint8_t charHeight = pgm_read_uint8_t_near(font + 4);
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
			drawCharInternal( runningX, y, *string, font );
			
			runningX += charWidth;
		}
		
        string = string + 1;
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
*/