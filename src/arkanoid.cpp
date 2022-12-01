#include <ArduinoAPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "fake_display.h"
#include "games.h"

static Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

boolean paused = false;

struct Brick
{
    bool active;
    int x;
    int y;
    int size_x;
    int size_y;
};

const int BRICK_COUNT = 10;

Brick bricks[BRICK_COUNT];

//player variables
int player_score = 0;
int player_h = 2;
int player_w = 10;
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
void arkanoid_setup() {

    for (int i = 0; i < BRICK_COUNT; i++){
        bricks[i].active = false;
    }

    display.begin();
    // init done

    // you can change the contrast around to adapt the display
    // for the best viewing!
    display.setContrast(75);

    display.display(); // show splashscreen
    delay(1000);
    display.clearDisplay();   // clears the screen and buffer

    //display.

    // text display tests
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.setCursor(0, 10);
    display.println("ARKANOID");
    //display.setTextColor(WHITE, BLACK); // 'inverted' text
    //display.println(3.141592);
    //display.setTextSize(2);
    //display.setTextColor(BLACK);
    //display.print("0x"); display.println(0xDEADBEEF, HEX);
    display.display();
    delay(1000);
}

///////////////////////////////////// LOOP
void arkanoid_loop() {

  if (1/*gb.update()*/) {
    display.clearDisplay();

    auto pFakeDisplay = FakeDisplay::getInstance();

    //move the player
    if (pFakeDisplay->isKeyPressed(FakeDisplay::eKey_Left)){
      player_x = max(0, player_x - player_vx);
    }
    if (pFakeDisplay->isKeyPressed(FakeDisplay::eKey_Right)) {
      player_x = min(LCDWIDTH - player_w, player_x + player_vx);
    }

    if (pFakeDisplay->isKeyPressed(FakeDisplay::eKey_Enter)) {
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

    for (int i = 0; i < BRICK_COUNT; i++)
    {
        if (bricks[i].active){
            // Collision detection
            // Collision from side
            // Collision from top/bottom
            //gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, player_x, player_y, player_w, player_h)) {
        }
    }

    //collision with the player
    if (false /*gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, player_x, player_y, player_w, player_h)*/) {
      ball_y = player_y;// + player_w;
      ball_vy = -ball_vy;
      //gb.sound.playTick();
    }
    /*
    //reset score when 10 is reached
    if ((player_score == 10) || (oponent_score == 10)) {
      player_score = 0;
      oponent_score = 0;
    }

    //move the oponent
    if ((oponent_y + (oponent_h / 2)) < (ball_y + (ball_size / 2))) { //if the ball is below the oponent
      oponent_y = oponent_y + oponent_vy; //move down
      oponent_y = min(LCDHEIGHT - oponent_h, oponent_y); //don't go out of the screen
    }
    else {
      oponent_y = oponent_y - oponent_vy; //move up
      oponent_y = max(0, oponent_y); //don't go out of the screen
    }

    */

    //draw the score
    //gb.display.fontSize = 2;
    //gb.display.cursorX = 15;
    //gb.display.cursorY = 16;
    //gb.display.print(player_score);
    display.setCursor(15,16);
    display.setTextSize(2);
    display.println(player_score);

    //gb.display.cursorX = 57;
    //gb.display.cursorY = 16;
    display.setCursor(57,16);
    //display.print(oponent_score);
    //gb.display.print(oponent_score);
    //draw the ball

    //gb.display.fillRect(ball_x, ball_y, ball_size, ball_size);
    display.fillRect(ball_x, ball_y, ball_size, ball_size, 1);
    //draw the player
    display.fillRect(player_x, player_y, player_w, player_h, 1);
    //gb.display.fillRect(player_x, player_y, player_w, player_h);
    //draw the oponent
    //display.fillRect(oponent_x, oponent_y, oponent_w, oponent_h, 1);
    //gb.display.fillRect(oponent_x, oponent_y, oponent_w, oponent_h);

    display.display();
  }
}