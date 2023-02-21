
#ifdef BUILD_FOR_X86
#include <ArduinoApi.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

#include "games.h"
#include "my_gui.h"
#include "menu.hpp"
//GUI gui;

void showWelcomeScreen()
{
    GUI &gui = GUI::getInstance();
    gui.animatedIntroScreen();
}

void showDemo()
{
    GUI &gui = GUI::getInstance();
    gui.demo();
}

void fce3()
{
    //std::cout << "Function " << __func__ << "called" << std::endl;
}

//Menu * pMenu = nullptr;


void menuSetup()
{
    GUI &gui = GUI::getInstance();
    gui.init();
    showWelcomeScreen();

    //Serial.begin(115200);

    Menu menu(gui.display);


    Menu::NumberInput numberInput = {"Backlight", 255, 10, 100};
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
    item.name = "Games";
    item.type = submenu;
    item.submenu = new Menu(gui.display);

    MenuItem submenuItem;
    submenuItem.type = functionCall;
    submenuItem.name = "Show demo";
    submenuItem.fce = showDemo;
    item.submenu->registerItem(submenuItem);

    //submenuItem.name = "2/6 Space Impact";
    submenuItem.name = "Space Impact";
    item.submenu->registerItem(submenuItem);

    submenuItem.name = "Arkanoid";
    submenuItem.fce = arkanoid_play;
    item.submenu->registerItem(submenuItem);

    submenuItem.name = "Crates";
    submenuItem.fce = super_crate_play;
    item.submenu->registerItem(submenuItem);

    submenuItem.name = "Race cars";
    item.submenu->registerItem(submenuItem);

    submenuItem.name = "Mario";
    submenuItem.fce = marduino_play;
    item.submenu->registerItem(submenuItem);

    submenuItem.name = "Pong";
    submenuItem.fce = pong_play;
    item.submenu->registerItem(submenuItem);

    submenuItem.name = "Maze";
    submenuItem.fce = maze_play;
    item.submenu->registerItem(submenuItem);

    // Register submenu
    menu.registerItem(item);

    // Submenu 2
    item.name = "Settings";
    item.type = submenu;
    item.submenu = new Menu(gui.display);

    //MenuItem submenuItem;
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

    // Register submenu
    menu.registerItem(item);

    // Submenu 3
    item.name = "Demo";
    item.type = submenu;
    item.submenu = new Menu(gui.display);

    //MenuItem submenuItem;
    submenuItem.type = functionCall;
    submenuItem.name = "Welcome screen";
    submenuItem.fce = showWelcomeScreen;
    item.submenu->registerItem(submenuItem);
    
    submenuItem.name = "Show demo";
    submenuItem.type = functionCall;
    submenuItem.fce = showDemo;
    item.submenu->registerItem(submenuItem);
    
    // Register submenu
    menu.registerItem(item);

    while (1)
    {
        if (gui.update())
        {
            menu.show();
            /*bRun = */ menu.update();
        }
    }
}


void menuLoop()
{
}
