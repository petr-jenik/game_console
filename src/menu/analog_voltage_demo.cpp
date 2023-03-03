#include "my_gui.h"

#include "analog_voltage_demo.hpp"

void show_analog_voltage()
{
    GUI& gui = GUI::getInstance();
    while(1)
    {
        if (gui.update())
        {
            gui.display.clearDisplay();
            gui.display.setTextSize(0);

            int voltageMilliV = gui.battery.voltageMilliV();

            String tmpString;
            tmpString.concat(voltageMilliV);
            tmpString.concat(" mV");

            gui.display.setTextColor(BLACK);
            gui.alignText(tmpString.c_str(), gui.display.width()/2,  gui.display.height() / 2, BLACK, GUI::eHorizontalCenterAlign);

            gui.display.display();
        }

        if (gui.isKeyPressed(Buttons::eKey_Back))
        {
            break;
        }
    }
}
