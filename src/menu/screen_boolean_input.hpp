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

    item.storage.select.options.push_back("OFF");
    item.storage.select.options.push_back("ON");
    item.storage.select.iSelected = (item.storage.boolValue) ? 1 : 0;

    screen_select_input(item);

    // TODO 
    //item.storage.boolValue = (item.storage.select.iSelected) ? true : false; 
}
