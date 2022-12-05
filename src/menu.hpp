#pragma once
#include <vector>
#include "WString.h"

#include <ArduinoAPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "fake_display.h"
#include "my_user_input.h"

#include <cstdint>

// TODO Refactor this whole file - it is SO BAD - too complex
// Ideas - reconsider division between MenuItem and Menu class. Maybe Menu class contains too many specific details.

enum MenuItemType
{
    functionCall,
    numberInput,
    stringInput,
    submenu    
};

// Forward declaration
class Menu;

struct MenuItem
{
    MenuItemType type;
    String name;
    int intValue;
    bool boolValue;
    String stringValue;
    void (*fce)();
    Menu* submenu;
};


class Menu
{
protected:
    Menu* mpActiveSubmenu;
    Menu* upperMenu;
    std::vector<MenuItem> menuItems;
    Adafruit_PCD8544& mrDisplay;
    unsigned int activeItemIndex = 0;
    const int cLineHeight = 12;
    const int cSliderWidth = 3;

public:
    Menu(Adafruit_PCD8544& rDisplay): mrDisplay(rDisplay), mpActiveSubmenu(this), upperMenu(nullptr)
    {
        // Disable text wrapping
        this->mrDisplay.setTextWrap(false);
    }

    void showSlider(int iCurrent, int cNumberOfItems)
    {
        if (cNumberOfItems == 0)
        {
            cNumberOfItems++;
        }

        auto cDisplayWidth = this->mrDisplay.width();

        int sliderHeight = this->mrDisplay.height();
        int sliderLen = sliderHeight / cNumberOfItems;
        int sliderYPos = map(iCurrent, 0, cNumberOfItems, 0, sliderHeight);
        this->mrDisplay.drawFastVLine(cDisplayWidth - 3, 0, sliderHeight, BLACK);
        this->mrDisplay.drawFastVLine(cDisplayWidth - 2, sliderYPos, sliderLen, BLACK);
        this->mrDisplay.drawFastVLine(cDisplayWidth - 1, 0, sliderHeight, BLACK);
    }

    void show()
    {
        // Limit output to N lines
        size_t numberOfScreenLines = this->mrDisplay.height() / cLineHeight;

        std::cout << "numberOfScreenLines:" << numberOfScreenLines << std::endl;
        size_t numberOfMenuItems = mpActiveSubmenu->menuItems.size();

        // THIS IS SO BAD - FIX IT!

        this->mrDisplay.clearDisplay();
        this->mrDisplay.setTextSize(0);

        for (size_t drawLineIdx = 0; drawLineIdx < min(numberOfScreenLines, numberOfMenuItems); drawLineIdx++)
        {
            size_t currentIndex = 0;
            if (numberOfMenuItems <= numberOfScreenLines)
            {
                currentIndex = drawLineIdx;
            }
            else
            {
                // Show current menu item in the middle of the screen and wrap from end of menu back to the start.
                currentIndex = (this->mpActiveSubmenu->activeItemIndex - (numberOfScreenLines / 2) + drawLineIdx + numberOfMenuItems) % numberOfMenuItems;
            }

            std::cout << currentIndex << std::endl;

            String lineText;
            lineText.concat((currentIndex == this->mpActiveSubmenu->activeItemIndex) ? ">" :  " ");
            //lineText.concat((int)currentIndex);
            //lineText.concat(": ");
            lineText.concat(this->mpActiveSubmenu->menuItems[currentIndex].name);

            if (currentIndex == this->mpActiveSubmenu->activeItemIndex)
            {
                this->mrDisplay.setTextColor(/*color*/ WHITE, /*background*/ BLACK);
                this->mrDisplay.writeFillRect(0, drawLineIdx * cLineHeight - 1, this->mrDisplay.width() - cSliderWidth, cLineHeight, BLACK);
            }
            else
            {
                this->mrDisplay.setTextColor(/* color*/ BLACK, /*background*/ WHITE);
            }
        
            this->mrDisplay.setCursor(2, drawLineIdx * cLineHeight);
            this->mrDisplay.print(lineText);
        }

        this->showSlider(this->mpActiveSubmenu->activeItemIndex, numberOfMenuItems);

        this->mrDisplay.display();
        //std::cout << "----------" << std::endl;
    }

    void clearItems()
    {
        this->mpActiveSubmenu->menuItems.clear();
    }

    void registerItem(MenuItem& item)
    {
        this->mpActiveSubmenu->menuItems.push_back(item);
    }

    //setPosition();
    //enter();
    // Return boolean with info wether this (sub)menu should be still running
    bool update()
    {
        bool bKeepOnRunning = true;

        // Local copy of button states - to avoid race conditions
        bool bBtnUp = UserInput::isKeyPressed(UserInput::eKey_Up);
        bool bBtnDown = UserInput::isKeyPressed(UserInput::eKey_Down);
        bool bBtnLeft = UserInput::isKeyPressed(UserInput::eKey_Left);
        bool bBtnRight = UserInput::isKeyPressed(UserInput::eKey_Right);
        bool bBtnEnter = UserInput::isKeyPressed(UserInput::eKey_Enter);
        bool bBtnBack = UserInput::isKeyPressed(UserInput::eKey_Back);

        if (bBtnBack)
        {
            bKeepOnRunning = false;
        }

        if (bBtnUp || bBtnDown)
        {
            size_t numberOfMenuItems = this->mpActiveSubmenu->menuItems.size();

            // Go up or down
            int menuStep = (bBtnDown) ? -1 : 1;

            if (numberOfMenuItems != 0)
            {
                this->mpActiveSubmenu->activeItemIndex = (this->mpActiveSubmenu->activeItemIndex + numberOfMenuItems + menuStep) % numberOfMenuItems;
            }
        }

        if (bBtnEnter)
        {
            if (this->mpActiveSubmenu->menuItems[this->activeItemIndex].type == functionCall)
            {
                if (this->mpActiveSubmenu->menuItems[this->activeItemIndex].fce != nullptr)
                {
                    this->mpActiveSubmenu->menuItems[this->activeItemIndex].fce();
                }
                else
                {
                    std::cout << "No function registered for this menu item" << std::endl;
                }
            }
            if (this->mpActiveSubmenu->menuItems[this->activeItemIndex].type == submenu)
            {
                this->mpActiveSubmenu->menuItems[this->activeItemIndex].submenu->upperMenu = this;
                this->mpActiveSubmenu = this->mpActiveSubmenu->menuItems[this->activeItemIndex].submenu;
                //std::cout << "Show submenu" << std::endl;
                /*
                while(1)
                {
                    delay(0); // Allow rest of the system to run
                    this->menuItems[this->activeItemIndex].submenu->show();
                    bool bSubmenuRunning = this->menuItems[this->activeItemIndex].submenu->update();
                    if (bSubmenuRunning == false)
                    {
                        // Break from the submenu
                        break;
                    }
                }*/
            }
        }

        if (bKeepOnRunning == false)
        {
            if (this->mpActiveSubmenu->upperMenu != nullptr)
            {
                this->mpActiveSubmenu = this->mpActiveSubmenu->upperMenu;
                return true;
            }
            return false;
        }
        else
        {
            return true;
        }
        //return bKeepOnRunning;
    }
    //{

    //}
};

/*
class Submenu: public Menu
{
public:
    Submenu(){};
};

*/