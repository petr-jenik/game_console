#pragma once

#ifdef BUILD_FOR_X86
#include <ArduinoAPI.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

#include "menu_item.hpp"
#include "my_gui.h"

/*
struct Select
{
    int iSelected;
    std::map<String, int> options;
};*/

void screen_number_input(MenuItem& item)
{
    GUI& gui = GUI::getInstance();
    while(1)
    {
        // TODO Remove all magical constants
        if (gui.update())
        {
            gui.display.clearDisplay();
            gui.display.setTextSize(0);

            gui.alignText(item.name.c_str(), LCDWIDTH / 2, 10, BLACK, GUI::eHorizontalCenterAlign);

            String labelMin;
            labelMin.concat(item.storage.number.min);
            gui.alignText(labelMin.c_str(), 5,  gui.display.height() / 2 + 10, BLACK, GUI::eHorizontalLeftAlign);

            String labelMax;
            labelMax.concat(item.storage.number.max);
            gui.alignText(labelMax.c_str(), gui.display.width() - 5,  gui.display.height() / 2 + 10, BLACK, GUI::eHorizontalRightAlign);

            String labelCurrentValue;
            labelCurrentValue.concat(item.storage.number.value);
            gui.alignText(labelCurrentValue.c_str(), gui.display.width() / 2,  gui.display.height() / 2 + 20, BLACK, GUI::eHorizontalCenterAlign);

            // Show vertical slider bar
            const int cSliderOffsetX = 5;
            const int cSliderLength = gui.display.width() - 2 * cSliderOffsetX; // Remove offset from left and from right side of the screen


            gui.display.writeFillRect(cSliderOffsetX, gui.display.height() / 2, cSliderLength, 2, BLACK);

            //gui.display.drawFastHLine(5, )
            // Show slider end points
            gui.display.drawFastVLine(cSliderOffsetX, (gui.display.height() / 2), 5, BLACK);
            gui.display.drawFastVLine(gui.display.width() - cSliderOffsetX, (gui.display.height() / 2), 5, BLACK);

            // Show moving slider
            int tmp = map(item.storage.number.value, item.storage.number.min, item.storage.number.max, cSliderOffsetX, cSliderOffsetX + cSliderLength);
            gui.display.drawFastVLine(tmp, (gui.display.height() / 2) - 10, 10, BLACK);
            int16_t x0 = tmp - 3;
            int16_t y0 = (gui.display.height() / 2) - 5;
            int16_t x1 = tmp + 3;
            int16_t y1 = (gui.display.height() / 2) - 5;
            int16_t x2 = tmp;
            int16_t y2 = (gui.display.height() / 2) - 1 ;
            gui.display.fillTriangle(x0, y0, x1, y1, x2, y2,BLACK);

            //this->showStatusBar();
            gui.display.display();
        }

        if (gui.isKeyPressed(Buttons::eKey_Back))
        {
            break;
        }

        if (gui.isKeyPressed(Buttons::eKey_Left))
        {
            if (item.storage.number.value > item.storage.number.min)
            {
                item.storage.number.value--;
            }
        }

        if (gui.isKeyPressed(Buttons::eKey_Right))
        {
            if (item.storage.number.value < item.storage.number.max)
            {
                item.storage.number.value++;
            }
        }

        // Call a callback function, if provided
        if (item.genericCallback != nullptr)
        {
            item.genericCallback(item);
        }
    }
}
