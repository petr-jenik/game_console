#pragma once
#include <vector>
#include <map>
#include "WString.h"

//#include <ArduinoAPI.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_PCD8544.h>

//#include "fake_display.h"
//#include "my_user_input.h"

#include <my_gui.h>
#include <menu_item.hpp>

#include <cstdint>

// TODO Refactor this whole file - it is SO BAD - too complex
// Ideas - reconsider division between MenuItem and Menu class. Maybe Menu class contains too many specific details.


class MenuViewer
{
    const int cLineHeight = 9;
    const int cSliderWidth = 3;

    const int cStatusBarHeight = 7;
    const int cStatusBarWidth; // gui.display.width()

    GUI& gui;

public:
    MenuViewer() : gui(GUI::getInstance()), cStatusBarWidth(GUI::getInstance().display.width())
    {
        // Disable text wrapping
        gui.display.setTextWrap(false);
    }

    void showSlider(int iCurrent, int cNumberOfItems)
    {
        if (cNumberOfItems == 0)
        {
            cNumberOfItems++;
        }

        auto cDisplayWidth = gui.display.width();

        int sliderHeight = gui.display.height() - this->cStatusBarHeight;
        int sliderLen = sliderHeight / cNumberOfItems;
        int sliderYPos = this->cStatusBarHeight + map(iCurrent, 0, cNumberOfItems, 0, sliderHeight);

        // Draw the actual slider
        gui.display.writeFillRect(cDisplayWidth - 4,  this->cStatusBarHeight, 4, sliderHeight, WHITE);
        gui.display.drawFastVLine(cDisplayWidth - 1, this->cStatusBarHeight, sliderYPos - this->cStatusBarHeight + 1, BLACK);
        gui.display.drawFastHLine(cDisplayWidth - 2, sliderYPos, 1, BLACK);
        gui.display.drawFastVLine(cDisplayWidth - 3, sliderYPos + 1, sliderLen - 1, BLACK);
        gui.display.drawFastHLine(cDisplayWidth - 2, sliderYPos + sliderLen, 1, BLACK);
        gui.display.drawFastVLine(cDisplayWidth - 1, sliderYPos + sliderLen, sliderHeight, BLACK);
    }

    void showStatusBar()
    {
        int bateryPercents = 50;
        int backlightStatePercents = 50;

        // Clear the whole status bar area
        gui.display.writeFillRect(0, 0, cStatusBarWidth, cStatusBarHeight, WHITE);

        int batteryX = cStatusBarWidth - 25;
        int batteryY = 0;
        int batterySizeX = 20;
        int batterySizeY = 5;

        // Draw the battery
        gui.display.drawRect(batteryX, batteryY, batterySizeX, batterySizeY, BLACK);
        gui.display.drawFastVLine(batteryX - 1, batteryY + 1, batterySizeY - 2, BLACK);
    }

    void show(Menu& menu)
    {
        // Clear the display
        gui.display.clearDisplay();
        gui.display.setTextSize(0);

        // Limit output to N lines
        size_t numberOfScreenLines = (gui.display.height() - this->cStatusBarHeight) / cLineHeight;

        //std::cout << "numberOfScreenLines:" << numberOfScreenLines << std::endl;
        size_t numberOfMenuItems = menu.mpActiveMenu->menuItems.size();

        // THIS IS SO BAD - FIX IT!

        for (size_t drawLineIdx = 0; drawLineIdx < min(numberOfScreenLines, numberOfMenuItems); drawLineIdx++)
        {

            // Convert current drawLineIdx (e.g. printing the second line from top) to the actual index in the array of menu items
            size_t currentIndex = 0;
            if (numberOfMenuItems <= numberOfScreenLines)
            {
                // Currently drawn items index matches index of a screen line
                currentIndex = drawLineIdx;
            }
            else
            {
                if (menu.mpActiveMenu->activeItemIndex <= numberOfScreenLines/2)
                {
                    currentIndex = drawLineIdx;
                }
                else if(menu.mpActiveMenu->activeItemIndex >= (numberOfMenuItems - numberOfScreenLines/2))
                {
                    // TODO - This is just some testing shit. Delete this whole branch
                    currentIndex = numberOfMenuItems - numberOfScreenLines + drawLineIdx;
                }
                else
                {
                    // Show current menu item in the middle of the screen and wrap from end of menu back to the start.
                    currentIndex = (menu.mpActiveMenu->activeItemIndex - (numberOfScreenLines / 2) + drawLineIdx + numberOfMenuItems) % numberOfMenuItems;
                }
            }

            // Get the line text
            String lineText = menu.mpActiveMenu->menuItems[currentIndex].getLineText();

            // Print the line
            if (currentIndex == menu.mpActiveMenu->activeItemIndex)
            {
                gui.display.setTextColor(/*color*/ WHITE, /*background*/ BLACK);
                gui.display.writeFillRect(0, this->cStatusBarHeight + 1 + drawLineIdx * cLineHeight - 1, gui.display.width()/* - cSliderWidth*/, cLineHeight, BLACK);
            }
            else
            {
                gui.display.setTextColor(/* color*/ BLACK, /*background*/ WHITE);
            }
            // + 1 sucks - it is there because the black rectangle around selected text is bigger than the text and therefore text cannot be printed on Y=0 as the rectangle would be drawn on Y=-1.
            gui.display.setCursor(2,this->cStatusBarHeight + 1 + drawLineIdx * cLineHeight);
            gui.display.print(lineText);
        }

        // Show the slider if all the lines cannot fit to the screen
        if (numberOfMenuItems > numberOfScreenLines)
        {
            this->showSlider(menu.mpActiveMenu->activeItemIndex, numberOfMenuItems);
        }

        // Show status bar on top
        this->showStatusBar();

        // Update the display
        gui.display.display();
    }


    // Return boolean with info wether this (sub)menu should be still running
    bool update(Menu& menu)
    {
        bool bKeepOnRunning = true;

        if (gui.buttons.pressed(Buttons::eKey_Back))
        {
            bKeepOnRunning = false;
        }

        if (gui.buttons.pressed(Buttons::eKey_Up) || gui.buttons.pressed(Buttons::eKey_Down))
        {
            size_t numberOfMenuItems = menu.mpActiveMenu->menuItems.size();

            // Go up or down
            int menuStep = (gui.buttons.pressed(Buttons::eKey_Down)) ? 1 : -1;

            if (numberOfMenuItems != 0)
            {
                menu.mpActiveMenu->activeItemIndex = (menu.mpActiveMenu->activeItemIndex + numberOfMenuItems + menuStep) % numberOfMenuItems;
            }
        }

        if (gui.buttons.pressed(Buttons::eKey_Enter))
        {
            // enter pressed on a submenu - go to the submenu
            if (menu.mpActiveMenu->menuItems[menu.mpActiveMenu->activeItemIndex].type == submenu)
            {
                //menu.mpActiveMenu->menuItems[menu.mpActiveMenu->activeItemIndex].submenu->upperMenu = this;
                menu.mpActiveMenu->menuItems[menu.mpActiveMenu->activeItemIndex].submenu->upperMenu  = menu.mpActiveMenu;
                menu.mpActiveMenu = menu.mpActiveMenu->menuItems[menu.mpActiveMenu->activeItemIndex].submenu;
            }
            else
            {
                menu.mpActiveMenu->menuItems[menu.mpActiveMenu->activeItemIndex].onEnter();
            }
        }

        // Go back from this menu - show upper menu if possible
        if (bKeepOnRunning == false)
        {
            if (menu.mpActiveMenu->upperMenu != nullptr)
            {
                menu.mpActiveMenu = menu.mpActiveMenu->upperMenu;
                return true;
            }
            return false;
        }
        else
        {
            return true;
        }
    }
};
