#pragma once

#include "my_gui.h"
#include <iostream>

class Star
{
private:
    int x_mult;
    int y;
    int depth;
    int xOffset;

public:

    void restartPosition()
    {
        this->x_mult = LCDWIDTH + random(LCDWIDTH/2);
        this->y = random(LCDHEIGHT);
        this->depth = random(1,3);
        std::cout << "Depth: " << this->depth << std::endl;
    }

    Star()
    {
        restartPosition();
        this->x_mult = random(LCDWIDTH);
    }

    void update()
    {
        if (this->x_mult > 0) {
            this->x_mult -= this->depth;
        }
        else
        {
            restartPosition();
        }
    }

    void draw(GUI & gui, uint16_t color)
    {
        gui.display.drawPixel(this->x_mult, this->y, color);
    }
};

class Startfield
{
private:
    static const size_t cStarCount = 30;
    Star mStars[cStarCount];

    uint16_t mStarColor;
    bool mbMoveEnabled;

public:
    Startfield()
    :mStarColor(BLACK),
    mbMoveEnabled(true)
    {}

    void setColor(uint16_t color)
    {
        this->mStarColor = color;
    }

    void move(bool bEnabled){
        this->mbMoveEnabled = bEnabled;
    }

    void update()
    {
        if (this->mbMoveEnabled == false)
        {
            return;
        }

        for (int i = 0; i < cStarCount; i++)
        {
            this->mStars[i].update();
        }
    }

    void draw(GUI & gui)
    {
        for (int i = 0; i < cStarCount; i++)
        {
            this->mStars[i].draw(gui, mStarColor);
        }
    }

};