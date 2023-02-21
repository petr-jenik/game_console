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


// PJ - Updated to be compatible with my old code base.
#pragma once

#ifdef BUILD_FOR_X86
#include <ArduinoApi.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

//#include "settings.c"



/*    bool isKeyPressed(Keys eKey);

    void processKeyStates(const char * rxMessage);

    void init();
}*/

class Buttons {
public:
    enum Keys 
    {
        eKey_FIRST = 0,
        eKey_Up = eKey_FIRST,
        eKey_Down = 1,
        eKey_Left = 2,
        eKey_Right = 3,
        eKey_Enter = 4,
        eKey_Back = 5,
        eKey_LAST = eKey_Back,
        eKey_COUNT,
        eKey_C, // Just for compatibility with Gamebuino
    };

    void begin();
    void update();
    bool pressed(Keys eKey);
    bool released(Keys eKey);
    bool held(Keys eKey, uint8_t time);
    bool repeat(Keys eKey, uint8_t period);
    uint8_t timeHeld(Keys eKey);
    uint8_t pins[eKey_COUNT];
    uint8_t states[eKey_COUNT];
};
