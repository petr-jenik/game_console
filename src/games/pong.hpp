#pragma once

#ifdef BUILD_FOR_X86
#include <ArduinoAPI.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

#include "my_gui.h"
#include "gameAPI.h"
//#include "games.h"

class Pong : public GameAPI
{
private:
    boolean paused = false;

    //player variables
    int player_score = 0;
    int player_h = 8;
    int player_w = 2;
    int player_x = 0;
    int player_y = (LCDHEIGHT - player_h) / 2;
    int player_vy = 2;

    //oponent variables
    int oponent_score = 0;
    int oponent_h = 8;
    int oponent_w = 2;
    int oponent_x = LCDWIDTH - oponent_w;
    int oponent_y = (LCDHEIGHT - oponent_h) / 2;
    int oponent_vy = 1;
    //ball variables
    int ball_size = 6;
    int ball_x = LCDWIDTH - ball_size - oponent_w - 1;
    int ball_y = (LCDHEIGHT - ball_size) / 2;
    int ball_vx = 2;
    int ball_vy = 2;

public:
    void setup()
    {
        // text display tests
        GUI& gui = gui.getInstance();

        gui.display.clearDisplay();
        gui.display.setTextSize(1);
        //gui.display.setTextColor(BLACK);
        //gui.display.setCursor(0, 10);
        gui.alignText("Pong", LCDWIDTH / 2, LCDHEIGHT / 2, BLACK, GUI::eHorizontalCenterAlign, GUI::eVerticalCenterAlign);
        gui.display.display();

        delay(1000);
    }


    void loop()
    {
        GUI& gui = GUI::getInstance();
        gui.display.clearDisplay();

        /*
        //pause the game if C is pressed
        if (gb.buttons.pressed(BTN_C)) {
          gb.titleScreen(F("Pong Solo"));
          gb.battery.show = false;
          gb.display.fontSize = 2;
        }
        */

        //move the player
        if (gui.isKeyPressed(Buttons::eKey_Up))
        {
            player_y = max(0, player_y - player_vy);
        }
        if (gui.isKeyPressed(Buttons::eKey_Down))
        {
            player_y = min(LCDHEIGHT - player_h, player_y + player_vy);
        }

        //move the ball
        ball_x = ball_x + ball_vx;
        ball_y = ball_y + ball_vy;

        //check for ball collisions
        //collision with the top border
        if (ball_y < 0)
        {
            ball_y = 0;
            ball_vy = -ball_vy;
            //gb.sound.playTick();
        }
        //collision with the bottom border
        if ((ball_y + ball_size) > LCDHEIGHT)
        {
            ball_y = LCDHEIGHT - ball_size;
            ball_vy = -ball_vy;
            //gb.sound.playTick();
        }
        
        //collision with the player
        if (gui.collideRectRect(ball_x, ball_y, ball_size, ball_size, player_x, player_y, player_w, player_h)) {
          ball_x = player_x + player_w;
          ball_vx = -ball_vx;
          //gb.sound.playTick();
        }

        //collision with the oponent
        if (gui.collideRectRect(ball_x, ball_y, ball_size, ball_size, oponent_x, oponent_y, oponent_w, oponent_h)) {
          ball_x = oponent_x - ball_size;
          ball_vx = -ball_vx;
          //gb.sound.playTick();
        }

        //collision with the left side
        if (ball_x < 0)
        {
            oponent_score = oponent_score + 1;
            //gb.sound.playCancel();
            ball_x = LCDWIDTH - ball_size - oponent_w - 1;
            ball_vx = -abs(ball_vx);
            ball_y = random(0, LCDHEIGHT - ball_size);
        }
        //collision with the right side
        if ((ball_x + ball_size) > LCDWIDTH)
        {
            player_score = player_score + 1;
            //gb.sound.playOK();
            ball_x = LCDWIDTH - ball_size - oponent_w - 16; //place the ball on the oponent side
            ball_vx = -abs(ball_vx);
            ball_y = random(0, LCDHEIGHT - ball_size);

        }
        //reset score when 10 is reached
        if ((player_score == 10) || (oponent_score == 10))
        {
            player_score = 0;
            oponent_score = 0;
        }
    

        if (1/*ball_vx > 0*/)
        {
            //move the oponent
            if ((oponent_y + (oponent_h / 2)) < (ball_y + (ball_size / 2)))
            { //if the ball is below the oponent
                oponent_y = oponent_y + oponent_vy; //move down
                oponent_y = min(LCDHEIGHT - oponent_h, oponent_y); //don't go out of the screen
            }
            else
            {
                oponent_y = oponent_y - oponent_vy; //move up
                oponent_y = max(0, oponent_y); //don't go out of the screen
            }
        }
        //draw the score
        //gb.display.fontSize = 2;
        //gb.display.cursorX = 15;
        //gb.display.cursorY = 16;
        //gb.display.print(player_score);
        gui.display.setCursor(15,16);
        gui.display.setTextSize(2);
        gui.display.println(player_score);

        //gb.display.cursorX = 57;
        //gb.display.cursorY = 16;
        gui.display.setCursor(57,16);
        gui.display.print(oponent_score);
        //gb.display.print(oponent_score);
        //draw the ball

        //gb.display.fillRect(ball_x, ball_y, ball_size, ball_size);
        gui.display.fillRect(ball_x, ball_y, ball_size, ball_size, 1);
        //draw the player
        gui.display.fillRect(player_x, player_y, player_w, player_h, 1);
        //gb.display.fillRect(player_x, player_y, player_w, player_h);
        //draw the oponent
        gui.display.fillRect(oponent_x, oponent_y, oponent_w, oponent_h, 1);
        //gb.display.fillRect(oponent_x, oponent_y, oponent_w, oponent_h);

        // Draw the middle line
        gui.drawDashedVLine(gui.display.width() / 2 - 1, 0, gui.display.height(), 4, 3);

        gui.display.display();
    }
};
