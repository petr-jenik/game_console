#include <ArduinoAPI.h>
#include "games.h"
#include "my_gui.h"
boolean paused = false;

#include "menu.hpp"

GUI gui;

void fce1()
{
    std::cout << "Function " << __func__ << "called" << std::endl;
}

void fce2()
{
    std::cout << "Function " << __func__ << "called" << std::endl;
}

void fce3()
{
    std::cout << "Function " << __func__ << "called" << std::endl;
}

void middleTextOnPosition(const char * pText, int x, int y, int color)
{
    //int textWidth = 0;
    //int textHeight = 0;
    int16_t upperLeftX, upperLeftY;
    uint16_t w,h;
    gui.display.getTextBounds(pText, 0, 0, &upperLeftX, &upperLeftY, &w, &h);
    std::cout << w << ", " << h << std::endl;

    //std::cout << (x - (w/2)) << ", " << y - (h/2) << std::endl;

    gui.display.setCursor(x - (w/2), y - (h/2));
    gui.display.println(pText);
}


void menu_setup()
{
    gui.init();

    gui.display.begin();
    // init done

    // you can change the contrast around to adapt the display
    // for the best viewing!
    gui.display.setContrast(75);

    gui.display.clearDisplay();
    gui.display.display(); // show splashscreen
    //delay(1000);
   // clears the screen and buffer

    //gui.display.


    // text display tests
    gui.display.drawRect(0, 0, LCDWIDTH - 1, LCDHEIGHT - 1, BLACK);
    gui.display.setTextSize(1);
    gui.display.setTextColor(BLACK);
    //gui.display.setCursor(10, 10);
    middleTextOnPosition("pwnduino", LCDWIDTH / 2, LCDHEIGHT / 2 - 10, BLACK);
    middleTextOnPosition("console", LCDWIDTH / 2, LCDHEIGHT / 2, BLACK);
    middleTextOnPosition("v0.1", LCDWIDTH / 2, LCDHEIGHT / 2 + 10, BLACK);

    //gui.display.println("SHITY");
    //gui.display.setCursor(16, 20);
    //gui.display.println("GAME");
    //gui.display.setCursor(8, 30);
    //gui.display.println("CONSOLE");
    //gui.display.setTextColor(WHITE, BLACK); // 'inverted' text
    //gui.display.println(3.141592);
    //gui.display.setTextSize(2);
    //gui.display.setTextColor(BLACK);
    //gui.display.print("0x"); gui.display.println(0xDEADBEEF, HEX);
    gui.display.display();
    delay(2000);


    Menu menu(gui.display);

    MenuItem item;
    // Submenu 1
    item.name = "Games";
    item.type = submenu;
    item.submenu = new Menu(gui.display);

    MenuItem submenuItem;
    submenuItem.type = functionCall;
    submenuItem.name = "Snake";
    submenuItem.fce = nullptr;
    item.submenu->registerItem(submenuItem);

    submenuItem.name = "Space Impact";
    item.submenu->registerItem(submenuItem);

    submenuItem.name = "Arkanoid";
    item.submenu->registerItem(submenuItem);

    submenuItem.name = "Tanks";
    item.submenu->registerItem(submenuItem);

    submenuItem.name = "Race cars";
    item.submenu->registerItem(submenuItem);

    submenuItem.name = "Mario";
    item.submenu->registerItem(submenuItem);

    // Register submenu
    menu.registerItem(item);

    // Submenu 2
    item.name = "Settings";
    item.type = submenu;
    item.submenu = new Menu(gui.display);

    //MenuItem submenuItem;
    submenuItem.type = functionCall;
    submenuItem.name = "Brightness";
    submenuItem.fce = nullptr;
    item.submenu->registerItem(submenuItem);
    submenuItem.name = "LCD Bias";
    item.submenu->registerItem(submenuItem);
    submenuItem.name = "LCD Contrast";
    item.submenu->registerItem(submenuItem);
    // Register submenu
    menu.registerItem(item);
/*
    item.type = functionCall;
    item.name = "ABC";
    item.fce = fce1;
    menu.registerItem(item);

    item.name = "123";
    item.fce = fce2;
    menu.registerItem(item);
    
    item.name = "456";
    item.fce = fce3;
    menu.registerItem(item);
    
    item.fce = nullptr;
    item.name = "LONG NAME";
    menu.registerItem(item);


    item.type = functionCall;
    item.fce = nullptr;
    item.name = "LONG NAME";
    menu.registerItem(item);
    menu.registerItem(item);
    menu.registerItem(item);
    menu.registerItem(item);
    menu.registerItem(item);
    menu.registerItem(item);
    menu.registerItem(item);
    menu.registerItem(item);
    menu.registerItem(item);
    menu.registerItem(item);
*/

    bool bRun = true;
    while(bRun)
    {
        menu.show();
        /*bRun = */ menu.update();
        delay(100);
    }
    std::cout << "Good bye!" << std::endl;
}


void menu_loop()
{
}

/*void me_loop()
{
    if (gui.update())
    {
        gui.display.clearDisplay();

        int y = 5;
        int size_x = 10;
        int size_y = 40;
        bool fill = true;
        int fillPattern = 1;

        for (int x = 0; x < LCDWIDTH - 10; x += 11)
        {
            draw_rect(x, y, size_x, size_y, fill, fillPattern++);
        }

        gui.display.display();
    }
}*/