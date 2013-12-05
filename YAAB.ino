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

#include "mainx.h"

void setup() 
{
#if defined SERIAL_DEBUG
    Serial.begin(9600);
        
    Serial.print("Size of EyeSettings: ");
    Serial.println(sizeof(EyeSettings));
        
    Serial.print("Size of MarkerTiming: ");
    Serial.println(sizeof(MarkerTiming));
        
    Serial.print("Size of MarkerSettings: ");
    Serial.println(sizeof(MarkerSettings));
        
    Serial.print("Size of CycleValues: ");
    Serial.println(sizeof(CycleValues));
        
    Serial.print("Size of MarkerProfile: ");
    Serial.println(sizeof(MarkerProfile));
        
    Serial.print("Size of uint8_t: ");
    Serial.println(sizeof(uint8_t));
        
    Serial.print("Size of uint16_t: ");
    Serial.println(sizeof(uint16_t));
        
    Serial.print("Size of uint32_t: ");
    Serial.println(sizeof(uint32_t));
#endif

    initMarker();

    
#if defined SERIAL_DEBUG
    Serial.println("Setup complete");
#endif
}

void loop()
{  
    loopMarker();
}
