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

#include "common.h"

//static uint32_t neutralizeTime = 0;
static int16_t  i2c_errors_count = 0;

#define INTERNAL_I2C_PULLUPS

#define I2C_SPEED 400000L     //100kHz normal mode, this value must be used for a genuine WMP

#define I2C_PULLUPS_ENABLE         PORTC |= 1<<4; PORTC |= 1<<5;   // PIN A4&A5 (SDA&SCL)
#define I2C_PULLUPS_DISABLE        PORTC &= ~(1<<4); PORTC &= ~(1<<5);

void i2c_init(void) 
{
	#if defined(INTERNAL_I2C_PULLUPS)
	I2C_PULLUPS_ENABLE
	#else
	I2C_PULLUPS_DISABLE
	#endif
	TWSR = 0;                                    // no prescaler => prescaler = 1
	TWBR = ((F_CPU / I2C_SPEED) - 16) / 2;   // change the I2C clock rate
	TWCR = 1<<TWEN;                              // enable twi module, no interrupt
}

void waitTransmissionI2C()
{
	uint16_t count = 255;
	while (!(TWCR & (1<<TWINT)))
	{
		count--;
		if (count==0)
		{
			//we are in a blocking state => we don't insist
			TWCR = 0;                  //and we force a reset on TWINT register
			//neutralizeTime = micros(); //we take a timestamp here to neutralize the value during a short delay
			i2c_errors_count++;
			break;
		}
	}
}

void i2c_rep_start(uint8_t address) 
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) ; // send REPEAT START condition
	waitTransmissionI2C();                       // wait until transmission completed
	TWDR = address;                              // send device address
	TWCR = (1<<TWINT) | (1<<TWEN);
	waitTransmissionI2C();                       // wail until transmission completed
}

void i2c_stop(void) 
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	//  while(TWCR & (1<<TWSTO));                // <- can produce a blocking state with some WMP clones
}

void i2c_write(uint8_t data ) 
{
	TWDR = data;                                 // send data to the previously addressed device
	TWCR = (1<<TWINT) | (1<<TWEN);
	waitTransmissionI2C();
}

void i2c_writeReg(uint8_t add, uint8_t reg, uint8_t val) 
{
	i2c_rep_start(add<<1); // I2C write direction
	i2c_write(reg);        // register selection
	i2c_write(val);        // value to write in register
	i2c_stop();
}


void i2c_OLED_send_cmd(uint8_t address, uint8_t command) 
{
	TWBR = 12;//TWBR = ((F_CPU / 400000L) - 16) / 2; // change the I2C clock rate
	i2c_writeReg(address, 0x80, (uint8_t)command);
}

void i2c_OLED_send_byte(uint8_t address, uint8_t val) 
{
	TWBR = 12;//TWBR = ((F_CPU / 400000L) - 16) / 2; // change the I2C clock rate
	i2c_writeReg(address, 0x40, (uint8_t)val);
}

