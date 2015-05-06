//
//  interrupts.cpp
//  YAAB
//
//  Created by Dan Silk on 6/05/2015.
//  Copyright (c) 2015 Silly Kat. All rights reserved.
//

#include "interrupts.h"
#include "marker.h"

///
/// Timer Tick
void onTimerTick()
{
    g_Marker.TimerTick();
}

///
/// ADC Conversion Complete
void onADCReadComplete()
{
    // Read the value ADC for a value between 0-255?
    //eyeCycleTask.SetCurrentEye(ADCH);
    
    g_Marker.UpdateEye(ADCH);
}