
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ssd1306_oled.h"

SSD1306_OLED display;

void setup() 
{
  display.Init(SSD1306_OLED::SPI_4WIRE);
}

bool flip = false;
void loop()
{  
  
  display.ClearDisplay();

  display.FillRect(10, 15, 108, 39, flip);
  flip = !flip;
  
  display.DisplayBuffer();

  delay(1000);
}
