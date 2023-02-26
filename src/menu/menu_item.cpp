
#include <vector>
#include <map>
#include <ArduinoAPI.h>

#include <menu_item.hpp>
#include <screen_number_input.hpp>
#include <screen_boolean_input.hpp>

void Menu::clearItems()
{
    menuItems.clear();
}

void Menu::registerItem(MenuItem& item)
{
    menuItems.push_back(item);
}

void MenuItem::onEnter()
{
    switch(this->type)
    {
        case functionCall:
        {
            if (this->fce != nullptr)
            {
                this->fce();
            }
            else
            {
                //std::cout << "No function registered for this menu item" << std::endl;
            }
            break;
        }
        case numberInput:
        {
            screen_number_input(*this);
            break;
        }
        case checkbox:
        {
            screen_boolean_input(*this);
            // Negate the value
            //this->storage.boolValue = !this->storage.boolValue;
            break;
        }
        case stringInput:
        {
            break;
        }

        //case submenu:
        default:
        {
            // Nothing to do here
            break;
        }
    }
}

// Get text description of the current line
String MenuItem::getLineText()
{
    String lineText ="";

    if (this->type == checkbox)
    {
        lineText.concat( this->storage.boolValue ? "x|" : " |");
    }

    lineText.concat(this->name);
    return lineText;
}
