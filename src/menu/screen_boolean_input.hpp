#pragma once

#ifdef BUILD_FOR_X86
#include <ArduinoAPI.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

#include "menu_item.hpp"
#include "my_gui.h"

#include "screen_item_select.hpp"

 
void screen_boolean_input(MenuItem& item)
{
    //Select select = {0, {"Item 1", "Item 2"}};//, "Item 3", "Item 4", "Item 5", "Item 6", "Item 7", "Item 8"}};
   
    std::vector<String> options;
    options.push_back("OFF");
    options.push_back("ON");
    // TODO - This is really bad. Stop abusing vector every time this screen is loaded.
    item.storage.select.options = options;

    // TODO -  Define some conversion functions from bool to input select and from input select to boolean. This sucks!
    item.storage.select.iSelected = (item.storage.boolValue) ? 1 : 0;

    screen_select_input(item);
}
