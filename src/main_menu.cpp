
#ifdef BUILD_FOR_X86
#include <ArduinoApi.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

#include "games.h"
#include "my_gui.h"
#include "menu.hpp"
//GUI gui;

#include "keyboard.hpp"

#include "FreeSansBoldOblique24pt7b.h"
// TODO List
// Solve gui.isKeyPressed vs gui.buttons.repeat vs gui.buttons.pressed
// Remove hardcoded wifi credentials
// Number input - show current value
// Number input - actually update something (if contrast is updated, call a function to use this new value)
// Number input - A: save B:cancel - implement this
// Boolean input - it adds options (ON, OFF) everytime the screen is called
// Boolean input - actually update something (if backlight is updated, call a function to use this new value)
// Boolean input - how to show whether boolean is True or False in the menu. Should it be shown with "X" or not?
// Boolean input - A: save B:cancel - implement this
// String input - implement keyboard
// Voltage measurement - stop measuring it every cycle - update e.g. once per second
// Add proper welcome screen (show an animated logo)
// Arkanoid - add score
// Arkanoid - add powerups (short player board, long player board, shooting)
// Arkanoid - add levels with different brick layout
// Implement Tower of Hanoi - with number of moves
// Slider is little off - bottom line (row) is missing
// Add wifi scanner
// Remote sw update - update texts. Maybe stop using showPopUp() and implement something directly for remote_sw_update
// Remote sw update - fix printing of IP address
//                  >>> [1761650880 >> (i*8) & 0xFF for i in range(4)]
//                  [192, 168, 0, 105]
// Add step-up converter 
// Add wifi deauther - try various ways, e.g. create an AP with the same SSID and same MAC address.

#include "analog_voltage_demo.hpp"

#include "remote_sw_update.hpp"


void animatedIntroScreen()
{
    GUI& gui = GUI::getInstance();
    gui.display.clearDisplay();

    /*
    gui.display.setTextWrap(true);
    //gui.display.setFont(&FreeSansBoldOblique24pt7b);
    int offset = 0;
    //for (int offset = 0; offset > -100; offset--)
    for (int fontSize = 10; fontSize > 0; fontSize--)
    {
        String text = "Petr je\n king";
        int16_t tmpX;
        int16_t tmpY;
        uint16_t w;
        uint16_t h;

        gui.display.clearDisplay();
        //gui.drawDashedVLine(0,0,20);
        gui.display.setTextSize(fontSize);
        gui.display.getTextBounds(text,0, 0, &tmpX, &tmpY, &w, &h);
        gui.display.setCursor(LCDWIDTH / 2, LCDHEIGHT / 2);

        gui.display.print("Petr je king");
        gui.display.display(); // show splashscreen
        delay(100);
    }
    delay(1000);
    gui.display.setFont(nullptr);
    */

    gui.display.clearDisplay();
    //gui.display.display(); // show splashscreen
    //delay(1000);

    // text display tests
    gui.display.drawRect(0, 0, LCDWIDTH - 1, LCDHEIGHT - 1, BLACK);
    gui.display.setTextSize(1);
    gui.display.setTextColor(BLACK);

    for (int32_t i = 0; i < LCDHEIGHT / 2; i++)
    {
        gui.display.drawRect(i, i, LCDWIDTH - 1 - 2*i, LCDHEIGHT - 1 - 2*i, BLACK);
        gui.display.display();
        delay(30);
    }

    for (int i = 0; i < LCDHEIGHT / 2; i++)
    {
        gui.display.drawRect(i, i, LCDWIDTH - 1 - 2*i, LCDHEIGHT - 1 - 2*i, WHITE);
        gui.display.setCursor(10, 10);
        gui.alignText("HackBox", LCDWIDTH / 2, LCDHEIGHT / 2 - 10, BLACK, GUI::eHorizontalCenterAlign);
        gui.alignText("console", LCDWIDTH / 2, LCDHEIGHT / 2, BLACK, GUI::eHorizontalCenterAlign);
        gui.alignText("v0.1", LCDWIDTH / 2, LCDHEIGHT / 2 + 10, BLACK, GUI::eHorizontalCenterAlign);

        gui.display.display();
        delay(30);
    }


    gui.display.display();
    delay(1000);
}


void showWelcomeScreen()
{
    animatedIntroScreen();
}

void showDemo()
{
    GUI &gui = GUI::getInstance();
    gui.display.setTextSize(1);

    const int cMaxCol = (Buttons::eKey_COUNT / 2);
    const int cMaxRow = 2;
    const int32_t cButtonWidth = (LCDWIDTH / cMaxCol);
    const int32_t cButtonHeight = (LCDHEIGHT / cMaxRow);

    while(1) {
        if (!gui.update()) {
            continue;
        }

        gui.display.clearDisplay();

        for (int32_t key = 0; key < Buttons::eKey_COUNT; key++)
        {
            int32_t iRow = key / cMaxCol;
            int32_t iCol = key % cMaxCol;

            // The number of frames during which the button has been held.
            uint8_t heldFramesCount = gui.buttons.timeHeld(static_cast<Buttons::Keys>(key));

            //bool bPressed = gui.buttons.pressed(static_cast<Buttons::Keys>(key));

            uint16_t buttonColor = (heldFramesCount > 0) ? BLACK : WHITE;
            uint16_t textColor = (heldFramesCount > 0) ? WHITE : BLACK;

            gui.display.writeFillRect(cButtonWidth * iCol, cButtonHeight * iRow, cButtonWidth, cButtonHeight, buttonColor);
            gui.display.setTextColor(textColor);
            
            char * buttonText = nullptr;

            switch(key)
            {
                case Buttons::eKey_Up:    buttonText = "UP";  break;
                case Buttons::eKey_Down:  buttonText = "DWN"; break;    
                case Buttons::eKey_Left:  buttonText = "L";   break;
                case Buttons::eKey_Right: buttonText = "R";   break;
                case Buttons::eKey_Enter: buttonText = "ENT"; break;
                case Buttons::eKey_Back:  buttonText = "BCK"; break;
                default:                  buttonText = "0";   break;
            }
            gui.alignText(buttonText, cButtonWidth * iCol + (cButtonWidth / 2), cButtonHeight * iRow + (cButtonHeight / 2), textColor, GUI::eHorizontalCenterAlign, GUI::eVerticalCenterAlign);
        }

        if ( gui.buttons.held(Buttons::eKey_Back, 10)) // TODO Move all magical constants to variables
        {
            break;
        }
        gui.display.display();
    }
}


// Advanced callback function for contrast configuration
void setContrast(MenuItem& item)
{
    GUI &gui = GUI::getInstance();
    gui.display.setContrast(item.storage.number.value);
}

void setBias(MenuItem& item)
{
    GUI &gui = GUI::getInstance();
    gui.display.setBias(item.storage.number.value);
}

void setBacklight(MenuItem& item)
{
    GUI &gui = GUI::getInstance();
    gui.setBacklight(item.storage.boolValue);
}

void menuSetup()
{
    GUI &gui = GUI::getInstance();
    gui.init();

    //screen_keyboard_demo();
    //screen_keyboard();
    
    showWelcomeScreen();

    gui.buttons.update();
    if (gui.buttons.pressed(Buttons::Keys::eKey_Enter))
    {
        gui.showPopUp("Remote SW\n update");
        remote_software_update();
    }

    //Serial.begin(115200);

    //Menu menu();
    Menu* menu = new Menu;

 
    /*numberInput.maxValue = 255;
    numberInput.minValue = 10;
    numberInput.name = 100;
    */
    /*while(1)
    {
        menu.numberInput(numberInput);
    }*/



    MenuItem item;

    // Submenu 1
    item.name = "WiFi magic";
    item.type = submenu;
    item.submenu = new Menu;

    MenuItem submenuItem;
    submenuItem.name = "WiFi scan";
    submenuItem.type = functionCall;
    submenuItem.fce = nullptr;
    item.submenu->registerItem(submenuItem);

    submenuItem.clear();

    submenuItem.name = "Deauth attack";
    submenuItem.type = functionCall;
    submenuItem.fce = nullptr;
    item.submenu->registerItem(submenuItem);

    // Register submenu
    menu->registerItem(item);

    // Submenu 1
    item.name = "Games";
    item.type = submenu;
    item.submenu = new Menu;

    //MenuItem submenuItem;
    /*
    submenuItem.name = "Keyboard test";
    submenuItem.type = functionCall;
    submenuItem.fce = showDemo;
    item.submenu->registerItem(submenuItem);
    */
    submenuItem.clear();
    submenuItem.name = "Arkanoid";
    submenuItem.type = functionCall;
    submenuItem.fce = arkanoid_play;
    item.submenu->registerItem(submenuItem);

    submenuItem.clear();
    submenuItem.type = functionCall;
    submenuItem.name = "Pong";
    submenuItem.fce = pong_play;
    item.submenu->registerItem(submenuItem);

    submenuItem.clear();
    submenuItem.type = functionCall;
    submenuItem.name = "Maze";
    submenuItem.fce = maze_play;
    item.submenu->registerItem(submenuItem);

    submenuItem.clear();
    submenuItem.name = "Tower of Hanoi";
    submenuItem.type = functionCall;
    submenuItem.fce = towers_play;
    item.submenu->registerItem(submenuItem);

/*
    //submenuItem.name = "2/6 Space Impact";
    submenuItem.clear();
    submenuItem.name = "Space Impact";
    submenuItem.type = functionCall;
    submenuItem.fce = nullptr;
    item.submenu->registerItem(submenuItem);

    submenuItem.clear();
    submenuItem.name = "Crates";
    submenuItem.type = functionCall;
    submenuItem.fce = nullptr;
    item.submenu->registerItem(submenuItem);
    
    submenuItem.clear();
    submenuItem.type = functionCall;
    submenuItem.fce = nullptr;
    submenuItem.name = "Race cars";
    item.submenu->registerItem(submenuItem);


    submenuItem.clear();
    submenuItem.type = functionCall;
    submenuItem.fce = nullptr;
    submenuItem.name = "Mario";
    //submenuItem.fce = marduino_play;
    item.submenu->registerItem(submenuItem);
*/
/*
    submenuItem.clear();
    submenuItem.name = "Space Impact";
    submenuItem.type = functionCall;
    submenuItem.fce = nullptr;
    item.submenu->registerItem(submenuItem);
*/
    submenuItem.clear();
    submenuItem.name = "Snake";
    submenuItem.type = functionCall;
    submenuItem.fce = snake_play;
    item.submenu->registerItem(submenuItem);

    // Register submenu
    menu->registerItem(item);

    // Submenu 2
    item.name = "Settings";
    item.type = submenu;
    item.submenu = new Menu;

    //MenuItem submenuItem;

    // Contrast
    submenuItem.clear();
    submenuItem.type = numberInput;
    submenuItem.name = "LCD Contrast";
    submenuItem.storage.number.max = 0x7F;
    submenuItem.storage.number.min = 0;
    submenuItem.storage.number.value = gui.display.getContrast();
    submenuItem.genericCallback = setContrast;
    item.submenu->registerItem(submenuItem);

    // Bias
    submenuItem.clear();
    submenuItem.type = numberInput;
    submenuItem.name = "LCD Bias";
    submenuItem.storage.number.max = 0x7;
    submenuItem.storage.number.min = 0;
    submenuItem.storage.number.value = gui.display.getBias();
    submenuItem.genericCallback = setBias;
    item.submenu->registerItem(submenuItem);


    // Backlight
    submenuItem.type = checkbox;
    submenuItem.name = "LCD Backlight";
    submenuItem.storage.boolValue = true;
    submenuItem.genericCallback = setBacklight;
    item.submenu->registerItem(submenuItem);
/*
    // Number input
    submenuItem.type = numberInput;
    submenuItem.name = "Number input";
    submenuItem.storage.number.max = 255;
    submenuItem.storage.number.min = 5;
    submenuItem.storage.number.value = 50;
    submenuItem.fce = nullptr;
    item.submenu->registerItem(submenuItem);
*/
    /*
    // Checkbox
    submenuItem.type = checkbox;
    submenuItem.name = "checkbox";
    submenuItem.fce = nullptr;
    item.submenu->registerItem(submenuItem);
    */

    /*
    // String input
    submenuItem.type = stringInput;
    submenuItem.name = "string input";
    submenuItem.fce = nullptr;
    item.submenu->registerItem(submenuItem);
    */

    // Read analog value
    submenuItem.type = functionCall;
    submenuItem.name = "Battery info";
    submenuItem.fce = show_analog_voltage;
    item.submenu->registerItem(submenuItem);
    
    // Remote SW update
    submenuItem.type = functionCall;
    submenuItem.name = "SW update";
    submenuItem.fce = remote_software_update;
    item.submenu->registerItem(submenuItem);

    /*
    submenuItem.type = checkbox;
    submenuItem.name = "Brightness";
    submenuItem.fce = nullptr;
    item.submenu->registerItem(submenuItem);
    submenuItem.name = "LCD Bias";
    item.submenu->registerItem(submenuItem);
    submenuItem.name = "LCD Contrast";
    item.submenu->registerItem(submenuItem);
    submenuItem.name = "Light";
    item.submenu->registerItem(submenuItem);
    */
    // Register submenu
    menu->registerItem(item);

    // Submenu 3
    item.name = "Demo";
    item.type = submenu;
    item.submenu = new Menu;

    //MenuItem submenuItem;
    submenuItem.type = functionCall;
    submenuItem.name = "Welcome screen";
    submenuItem.fce = showWelcomeScreen;
    item.submenu->registerItem(submenuItem);
    
    submenuItem.name = "Keyboard test";
    submenuItem.type = functionCall;
    submenuItem.fce = showDemo;
    item.submenu->registerItem(submenuItem);
    
    // Register submenu
    menu->registerItem(item);

    MenuViewer menuViewer;

    while (1)
    {
        if (gui.update())
        {
            menuViewer.show(*menu);
            /*bRun = */ menuViewer.update(*menu);
        }
    }
}


void menuLoop()
{
}
