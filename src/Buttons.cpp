/*
 * (C) Copyright 2014 Aurélien Rodot. All rights reserved.
 *
 * This file is part of the Gamebuino Library (http://gamebuino.com)
 *
 * The Gamebuino Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "Buttons.h"

#ifdef BUILD_FOR_X86
#include <ArduinoApi.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

void Buttons::begin()
{
    pins[eKey_Up] = 25;
    pins[eKey_Down] = 13;
    pins[eKey_Left] = 33;
    pins[eKey_Right] = 27;
    pins[eKey_Enter] = 12;
    pins[eKey_Back] = 14;

    for (int iButton = 0; iButton < sizeof(pins) / sizeof(pins[0]); iButton++)
    {
        pinMode(pins[iButton], INPUT_PULLUP);
    }
}

/*
 * reads each button states and store it
 */
void Buttons::update()
{
    for (int iButton = 0; iButton < eKey_COUNT; iButton++) {
        //pinMode(pins[iButton], INPUT_PULLUP); //enable internal pull up resistors
        if (digitalRead(pins[iButton]) == LOW) { //if button pressed
            states[iButton]++; //increase button hold time
        } else {
            if (states[iButton] == 0)//button idle
                continue;
            if (states[iButton] == 0xFF)//if previously released
                states[iButton] = 0; //set to idle
            else
                states[iButton] = 0xFF; //button just released
        }
        //pinMode(pins[iButton], INPUT); //disable internal pull up resistors to save power
    }
}

/*
 * Returns true when 'button' is pressed.
 * The button has to be released for it to be triggered again.
 */
boolean Buttons::pressed(Keys eKey) {
    if (states[eKey] == 1)
        return true;
    else
        return false;
}

/*
 * return true if 'button' is released
 */
boolean Buttons::released(Keys eKey) {
    if (states[eKey] == 0xFF)
        return true;
    else
        return false;
}

/**
 * returns true ONCE when 'button' is held for 'time' frames
 * @param button The button's ID
 * @param time How much frames button must be held, between 1 and 254.
 * @return true when 'button' is held for 'time' frames
 */
boolean Buttons::held(Keys eKey, uint8_t time){
    if(states[eKey] == (time+1))
        return true;
    else
        return false;
}

/**
 * returns true every 'period' frames when 'button' is held
 * @param button The button's ID
 * @param period How much frames button must be held, between 1 and 254.
 * @return true if the button is held for the given time
 */
boolean Buttons::repeat(Keys eKey, uint8_t period) {
    if (period <= 1) {
        if ((states[eKey] != 0xFF) && (states[eKey]))
            return true;
    } else {
        if ((states[eKey] != 0xFF) && ((states[eKey] % period) == 1))
            return true;
    }
    return false;
}

/**
 * 
 * @param button The button's ID
 * @return The number of frames during which the button has been held.
 */
uint8_t Buttons::timeHeld(Keys eKey){
    if(states[eKey] != 0xFF)
        return states[eKey];
    else
        return 0;
    
}