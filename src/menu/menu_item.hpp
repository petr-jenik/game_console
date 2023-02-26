#pragma once

#include <vector>
#include <map>
#include <ArduinoAPI.h>

enum MenuItemType
{
    functionCall,
    numberInput,
    checkbox,
    stringInput,
    submenu
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
};

class MenuItem
{
public:
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
