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

#include "Task.h"

Task::Task(TaskConditionMet conditionMet)
{
    onConditionMet = conditionMet;
    m_Enabled = false;
}

Task::~Task(void) 
{ 
}

void Task::Update()
{
    if(!m_Enabled) return;

    UpdateInternal();

    if(IsConditionMet())
    {
        m_Enabled = false;

        if(onConditionMet != 0)
            onConditionMet();
            
        ConditionMet();
    }
}

void Task::ConditionMet() 
{
}

void Task::Stop() 
{  
    m_Enabled = false; 
}

void Task::Reset() 
{  
    m_Enabled = true; 
}

