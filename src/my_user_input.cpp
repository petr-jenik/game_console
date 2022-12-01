#include "my_user_input.h"
#include <cstring>
#include <iostream>

static bool gKeyStates[UserInput::Keys::eKey_COUNT] = {0};

bool UserInput::isKeyPressed(UserInput::Keys eKey)
{
    bool retval = false;
    if ((eKey >= eKey_FIRST) && (eKey <= eKey_LAST)){
        retval = gKeyStates[eKey];
    }
    return retval;
}

void UserInput::processKeyStates(const char * rxMessage)
{
    if (strlen(rxMessage) != (UserInput::eKey_COUNT))
    {
        std::cout << "Invalid state of keys received" << std::endl;
        return;
        }

        bool retval = false;
        for (int keyIndex = static_cast<int>(UserInput::eKey_FIRST); keyIndex <= static_cast<int>(UserInput::eKey_LAST); keyIndex++)
        {
            char c = rxMessage[keyIndex];

            if ((c != '0') && (c != '1'))
            {
                std::cout << "Invalid state of keys received" << std::endl;
                return;
            }

            gKeyStates[keyIndex] = (c == '1');
        }
    }