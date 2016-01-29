/*
YAAB - Yet Another Autococker Board
Arduino based autococker board developed around the platform and ATMEL AVR
chips

Copyright (C) 2015  Dan Silk

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
#include <util/delay.h>

#include "crius_oled.h"
#include "font.h"
#include "common.h"

#include "../pins.h"
#include "../i2c.h"

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

///
/// Drawing Functions

void CRIUS_OLED::FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool fill)
{
	for (uint8_t i = x; i < x + w; i++)
	{
		for (uint8_t j = y; j < y + h; j++)
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
	uint8_t runningX = x;

	while (*string)
	{
		if (*string == '\n')
		{
			y += charHeight;
			runningX = x;
		}
		//else if (*string == ' ')
		//{
		//	runningX += charWidth;
		//}
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
	int pageIdx = bitIdx / 8;
	uint8_t pixelIdx = bitIdx % 8;

	if (draw)
		buffer[pageIdx] |= 1 << pixelIdx;
	else
		buffer[pageIdx] &= ~(1 << pixelIdx);

	m_IsDirty = true;
}

void CRIUS_OLED::DrawCharInternal(uint8_t x, uint8_t y, const char character, const uint8_t * font)
{
	uint8_t startChar = pgm_read_byte_near(font + 2);
	uint8_t charWidth = pgm_read_byte_near(font + 5);
	uint8_t charHeight = pgm_read_byte_near(font + 4);

	uint8_t bytesPerCol = (charHeight / 8) + 1;
	int16_t charIdx = ((character - startChar) * (charWidth * bytesPerCol)) + 6;

	for (uint8_t bytes = 0; bytes < charWidth; bytes++)
	{
		for (uint8_t cols = 0; cols < bytesPerCol; cols++)
		{
			uint8_t maxBits = charHeight - (cols * 8);
			uint16_t offset = charIdx + cols + (bytes * bytesPerCol);
			uint8_t charByte = pgm_read_byte_near(font + offset);

			if (maxBits > 8)
				maxBits = 8;

			for (uint8_t bits = 0; bits < maxBits; bits++)
			{
				bool bitValue = charByte & (1 << bits);

				this->DrawPixel(x + bytes, y + (cols * 8) + bits, bitValue);
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

void CRIUS_OLED::DrawGraphic(uint8_t graphicIndex)
{
	int index = graphicIndex * LCD_BUFFER_SIZE + 6;

	memcpy_P(buffer, &Graphics[index], LCD_BUFFER_SIZE);

	DisplayBuffer();
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
	m_IsDirty = false;
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
	if (!m_IsDirty) return;

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
