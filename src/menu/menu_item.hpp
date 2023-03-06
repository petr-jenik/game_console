#pragma once

#include <vector>
#include <map>

#ifdef BUILD_FOR_X86
#include <ArduinoAPI.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

enum MenuItemType
{
    functionCall,
    numberInput,
    checkbox,
    stringInput,
    submenu
};

struct Select
{
    size_t iSelected;
    std::vector<String> options;
};


// Forward declaration
class MenuItem;


class Menu
{
public:
    Menu* mpActiveMenu;
    Menu* upperMenu;
    std::vector<MenuItem> menuItems;
    unsigned int activeItemIndex = 0;

    // Constructor
    Menu() :mpActiveMenu(this), upperMenu(nullptr){};

    void clearItems();
    void registerItem(MenuItem& item);
};

struct NumberValue
{
    int min = 0;
    int max = 1000;
    int value = 0;
};

/*
struct ValueSelect
{
    int count;
    int selectedIndex;
    String* items;
};*/

struct Storage
{
    NumberValue number;
    bool boolValue = false;
    String stringValue = "";
    Select select;
};

class MenuItem
{
public:

    void clear()
    {
        this->type = functionCall;
        this->name = "UNDEFINED";
        this->storage.boolValue = false;
        this->storage.number.max=0;
        this->storage.number.min=0;
        this->storage.number.value=0;
        this->storage.select.iSelected=0;
        this->storage.select.options.clear();
        this->storage.stringValue = "";
        this->fce = nullptr;
        this->genericCallback = nullptr;
        this->submenu = nullptr;
    }

    MenuItemType type;
    String name;
    Storage storage;

    // Enter key pressed on the menu item
    void onEnter();

    // Get text description of the current line
    String getLineText();

    void (*fce)(); // Simple callback function
    void (*genericCallback)(MenuItem&); // Advanced callback function
    Menu* submenu; // Menu item might point directly to a submenu

    // Constructor
    MenuItem()
    :fce(nullptr),
    genericCallback(nullptr),
    submenu(nullptr)
    {};
};
