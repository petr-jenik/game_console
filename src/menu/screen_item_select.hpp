#pragma once

#ifdef BUILD_FOR_X86
#include <ArduinoAPI.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

#include "menu_item.hpp"
#include "my_gui.h"

#include <iostream>

#define TRACK // std::cout << __LINE__ << std::endl

void screen_select_input(MenuItem& item)
{
    TRACK;
    //Select select = {0, {"Item 1", "Item 2"}};//, "Item 3", "Item 4", "Item 5", "Item 6", "Item 7", "Item 8"}};

    const int cLineHeight = 10;
    TRACK;
    size_t tmpSelectedIdx = item.storage.select.iSelected;
    const size_t cNumberOfItems = item.storage.select.options.size();
    GUI& gui = GUI::getInstance();
    TRACK;
    while(1)
    {
        TRACK;
        // TODO Remove all magical constants
        if (gui.update())
        {
            TRACK;
            gui.display.clearDisplay();
            gui.display.setTextSize(0);

            gui.alignText(item.name.c_str(), LCDWIDTH / 2, 1, BLACK, GUI::eHorizontalCenterAlign, GUI::eVerticalBottomAlign);

// Magic starts here -------------------------------------------------

        // Limit output to N lines
            size_t numberOfScreenLines = 3;

            // THIS IS SO BAD - FIX IT!

            for (size_t drawLineIdx = 0; drawLineIdx < min(numberOfScreenLines, cNumberOfItems); drawLineIdx++)
            {

                // Convert current drawLineIdx (e.g. printing the second line from top) to the actual index in the array of menu items
                size_t currentIndex = 0;
                if (cNumberOfItems <= numberOfScreenLines)
                {
                    // Currently drawn items index matches index of a screen line
                    currentIndex = drawLineIdx;
                }
                else
                {
                    if (tmpSelectedIdx <= numberOfScreenLines/2)
                    {
                        currentIndex = drawLineIdx;
                    }
                    else if(tmpSelectedIdx >= (cNumberOfItems - numberOfScreenLines/2))
                    {
                        // TODO - This is just some testing shit. Delete this whole branch
                        currentIndex = cNumberOfItems - numberOfScreenLines + drawLineIdx;
                    }
                    else
                    {
                        // Show current menu item in the middle of the screen and wrap from end of menu back to the start.
                        currentIndex = (tmpSelectedIdx - (numberOfScreenLines / 2) + drawLineIdx + cNumberOfItems) % cNumberOfItems;
                    }
                }

                TRACK;
                // Get the line text
                //String lineText = menu.mpActiveMenu->menuItems[currentIndex].getLineText();


                // Print the line
                if (currentIndex == tmpSelectedIdx)
                {
                    gui.display.setTextColor(/*color*/ WHITE, /*background*/ BLACK);
                    gui.display.writeFillRect(0, (drawLineIdx+1) * cLineHeight - 1, gui.display.width()/* - cSliderWidth*/, cLineHeight, BLACK);
                }
                else
                {
                    gui.display.setTextColor(/* color*/ BLACK, /*background*/ WHITE);
                }
                
                gui.alignText(item.storage.select.options[currentIndex].c_str(), 10, (drawLineIdx+1) * cLineHeight, BLACK/*color not used*/, GUI::eHorizontalLeftAlign, GUI::eVerticalBottomAlign);
                // + 1 sucks - it is there because the black rectangle around selected text is bigger than the text and therefore text cannot be printed on Y=0 as the rectangle would be drawn on Y=-1.
                //gui.display.setCursor(2,this->cStatusBarHeight + 1 + drawLineIdx * cLineHeight);
                //gui.display.print(lineText);
            }

        // TODO Add slider
// Magic ends here --------------------------------------------------

            //gui.display.setTextSize(0);
            gui.setColor(BLACK);
            gui.alignText("A:OK B:Cancel", gui.display.width() /2,  gui.display.height() + 1 , BLACK, GUI::eHorizontalCenterAlign, GUI::eVerticalTopAlign);

            //this->showStatusBar();
            gui.display.display();
        }

        if (gui.buttons.pressed(Buttons::eKey_Back))
        {
            break;
        }
        
        if (gui.buttons.pressed(Buttons::eKey_Enter))
        {
            item.storage.select.iSelected = tmpSelectedIdx;
            if (item.type == checkbox)
            {
                // DIRTY HACK how to force input screen to behave like a checkbox
                // index 0 should be always "OFF"
                item.storage.boolValue = (tmpSelectedIdx == 0) ? false : true; 
            }
            break;
        }

        if (gui.buttons.pressed(Buttons::eKey_Up))
        {
            tmpSelectedIdx = (tmpSelectedIdx + cNumberOfItems - 1) % cNumberOfItems;
        }

        if (gui.buttons.pressed(Buttons::eKey_Down))
        {
            tmpSelectedIdx = (tmpSelectedIdx + 1) % cNumberOfItems;
        }
        TRACK;
    }


    // Call a callback function, if provided
    if (item.genericCallback != nullptr)
    {
        item.genericCallback(item);
    }
}
