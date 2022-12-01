#pragma once

namespace UserInput
{
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
        eKey_COUNT
    };

    bool isKeyPressed(Keys eKey);

    void processKeyStates(const char * rxMessage);
}