#pragma once

#include <ArduinoAPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "fake_display.h"
#include "my_user_input.h"

#include <cstdint>

//using namespace::FakeDisplay;

class GUI
{
public:

    GUI()
    :display(Adafruit_PCD8544(7, 6, 5, 4, 3)) // TODO Solve hardcoded pin numbers
    {};

    void init()
    {
        this->display.initDisplay();
    }

    bool update()
    {
        return true;
        // TODO Implement here
    }

    bool collidePointRect(int16_t x1, int16_t y1 ,int16_t x2 ,int16_t y2, int16_t w, int16_t h){
        if((x1>=x2)&&(x1<x2+w)){
            if((y1>=y2)&&(y1<y2+h)){
                return true;
            }
        }
        return false;
    }

    bool collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1 ,int16_t x2 ,int16_t y2, int16_t w2, int16_t h2)
    {
        return !( x2     >=  x1+w1  || 
            x2+w2  <=  x1     || 
            y2     >=  y1+h1  ||
            y2+h2  <=  y1     );
    }

    bool isKeyPressed(UserInput::Keys eKey)
    {
        return UserInput::isKeyPressed(eKey);
    }

    /*
    void drawTextOnLine(int lineIndex, String& lineString)
    {
        display.setCursor(2, i * 10);
        display.print(lineString);
    }*/

    Adafruit_PCD8544 display;

};