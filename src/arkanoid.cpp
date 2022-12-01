#include <ArduinoAPI.h>
#include "games.h"
#include "my_gui.h"
boolean paused = false;

GUI gui;

struct Brick
{
    bool active;
    int x;
    int y;

};

const int BRICK_ROWS = 6;
const int BRICK_COLS = 6;
const int BRICK_COUNT = BRICK_ROWS * BRICK_COLS;

const int brick_size_x = LCDWIDTH / (BRICK_COLS + 1);
const int brick_size_y = 1; //LCD_HEIGHT / (2 * (BRICK_ROWS + 1));

Brick bricks[BRICK_COUNT];

//player variables
int player_score = 0;
int player_h = 6;
int player_w = 20;
int player_x = (LCDWIDTH - player_w) / 2;
int player_y = (LCDHEIGHT - player_h);

int player_vx = 2;

//ball variables
int ball_size = 2;
int ball_x = (LCDWIDTH - ball_size) / 2;
int ball_y = (LCDHEIGHT - ball_size) / 2;
int ball_vx = 1;
int ball_vy = 1;

extern const byte font5x7[]; //get the default large font

///////////////////////////////////// SETUP
void arkanoid_setup()
{
    for (int i = 0; i < BRICK_COUNT; i++)
    {
        bricks[i].active = false;
    }

    for (int i = 0; i < BRICK_COLS; i++)
    {
        for (int j = 0; j < BRICK_ROWS; j++)
        {
            Brick & rBrick = bricks[i * BRICK_ROWS + j];
            rBrick.active = true;
            rBrick.x = 5 + i * brick_size_x + i; // TODO This is shit
            rBrick.y = 5 + j * brick_size_y + j; // TODO This is shit
        }
    }

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
    gui.display.setTextSize(1);
    gui.display.setTextColor(BLACK);
    gui.display.setCursor(10, 10);
    gui.display.println("ARKANOID");
    //gui.display.setTextColor(WHITE, BLACK); // 'inverted' text
    //gui.display.println(3.141592);
    //gui.display.setTextSize(2);
    //gui.display.setTextColor(BLACK);
    //gui.display.print("0x"); gui.display.println(0xDEADBEEF, HEX);
    gui.display.display();
    delay(1000);
}

///////////////////////////////////// LOOP
void arkanoid_loop()
{
    if (gui.update())
    {
        gui.display.clearDisplay();

        //move the player
        if (gui.isKeyPressed(UserInput::eKey_Left)) {
            player_x = max(0, player_x - player_vx);
        }
        if (gui.isKeyPressed(UserInput::eKey_Right)) {
            player_x = min(LCDWIDTH - player_w, player_x + player_vx);
        }

        if (gui.isKeyPressed(UserInput::eKey_Enter)) {
            //player_y = min(LCDWIDTH - player_x, player_x + player_vx);
            // Fire !
        }

        //move the ball
        ball_x = ball_x + ball_vx;
        ball_y = ball_y + ball_vy;

        //check for ball collisions
        //collision with the top border
        if (ball_y < 0) {
            ball_y = 0;
            ball_vy = -ball_vy;
            //gb.sound.playTick();
        }
        //collision with the bottom border
        if ((ball_y + ball_size) > LCDHEIGHT) {
            ball_y = LCDHEIGHT - ball_size;
            ball_vy = -ball_vy;
            //gb.sound.playTick();
        }

        //collision with the left side
        if (ball_x < 0) {
            ball_x = 0;
            ball_vx = -ball_vx;
            
            //ball_y = random(0, LCDHEIGHT - ball_size);
        }

        //collision with the right side
        if ((ball_x + ball_size) > LCDWIDTH) {
            ball_x = LCDWIDTH - ball_size;
            ball_vx = -ball_vx;
        }

        // Collision detection with bricks
        
        for (int i = 0; i < BRICK_COUNT; i++)
        {   
            Brick& rBrick = bricks[i];
            if (rBrick.active)
            {
                if (gui.collideRectRect(ball_x,
                                        ball_y, 
                                        ball_size,
                                        ball_size,
                                        rBrick.x,
                                        rBrick.y,
                                        brick_size_x,
                                        brick_size_y))
                {
                    rBrick.active = false;
                    ball_vy = -ball_vy;
                }
                // Collision detection
                // Collision from side
                // Collision from top/bottom
                //gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, player_x, player_y, player_w, player_h)) {
            }
        }

        //collision with the player
        if (gui.collideRectRect(ball_x, ball_y, ball_size, ball_size, player_x, player_y, player_w, player_h))
        {
            ball_y = player_y - player_h;
            ball_vy = -abs(ball_vy);
            //gb.sound.playTick();
        }
        /*
        //reset score when 10 is reached
        if ((player_score == 10) || (oponent_score == 10)) {
          player_score = 0;
          oponent_score = 0;
        }
        */

        //draw the score
        //gui.display.setCursor(15,16);
        //gui.display.setTextSize(2);
        //gui.display.println(player_score);

        //gui.display.setCursor(57,16);
        //gui.display.print(oponent_score);

        //draw the ball
        gui.display.fillRect(ball_x, ball_y, ball_size, ball_size, 1);
        //draw the player
        gui.display.fillRect(player_x, player_y, player_w, player_h, 1);

        //draw all the boxes
        for (int i = 0; i < BRICK_COUNT; i++)
        {
            Brick& brick = bricks[i];
            if (brick.active)
            {
                gui.display.fillRect(brick.x, brick.y, brick_size_x, brick_size_y, 1);
            }
        }
        //gui.display.fillRect(oponent_x, oponent_y, oponent_w, oponent_h, 1);
        //gb.gui.display.fillRect(oponent_x, oponent_y, oponent_w, oponent_h);

        gui.display.display();
    }
}