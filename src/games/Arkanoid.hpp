#include "my_gui.h"

class Arkanoid : public GameAPI
{
public:
    struct Brick
    {
        bool active;
        int x;
        int y;

    };

    static const int cBrickRowCount = 4;
    static const int cBrickColsCount = 10;
    static const int cBrickCount = cBrickRowCount * cBrickColsCount;

    static const int cBrickSizeWidth = LCDWIDTH / (cBrickColsCount + 1);
    static const int cBrickSizeHeight = 2; //LCD_HEIGHT / (2 * (cBrickRowCount + 1));

    Brick bricks[cBrickCount];

    static const int cBrickOffsetFromLeft = cBrickSizeWidth / 2;
    static const int cBrickOffsetFromTop = 5;

    //player variables
    int player_score = 0;
    static const int cPlayerHeight = 2;
    static const int cPlayerWidth = 10;
    static const int cPlayerPixelsAboveTheGround = 4;
    int playerPosX = (LCDWIDTH - cPlayerWidth) / 2;
    int playerPosY = (LCDHEIGHT - cPlayerHeight - cPlayerPixelsAboveTheGround);

    int playerVelocityX = 2;

    //ball variables
    static const int cBallSize = 3;
    int ballPosX = playerPosX + (cPlayerWidth / 2) ; //(LCDWIDTH - cBallSize) / 2;
    int ballPosY = playerPosY - cBallSize;; //(LCDHEIGHT - cBallSize) / 2;
    int ballVelocityX = 1;
    int ballVelocityY = -1;

    bool bIsBallMoving = false; // Ball should be stuck to the player at the beginning
    bool bHasFire = false;

    //extern const byte font5x7[]; //get the default large font

    void restoreDefaultValues()
    {
        // Player variables
        playerPosX = (LCDWIDTH - cPlayerWidth) / 2;
        playerPosY = (LCDHEIGHT - cPlayerHeight - cPlayerPixelsAboveTheGround);

        playerVelocityX = 2;

        //ball variables
        ballPosX = playerPosX + (cPlayerWidth / 2) ; //(LCDWIDTH - cBallSize) / 2;
        ballPosY = playerPosY - cBallSize;; //(LCDHEIGHT - cBallSize) / 2;
        ballVelocityX = 1;
        ballVelocityY = -1;

        bIsBallMoving = false; // Ball should be stuck to the player at the beginning
        bHasFire = false;

        for (int i = 0; i < cBrickCount; i++)
        {
            bricks[i].active = false;
        }

        for (int i = 0; i < cBrickColsCount; i++)
        {
            for (int j = 0; j < cBrickRowCount; j++)
            {
                Brick & rBrick = bricks[i * cBrickRowCount + j];
                rBrick.active = true;
                rBrick.x = cBrickOffsetFromLeft + i * cBrickSizeWidth + i; // TODO This is shit
                rBrick.y = cBrickOffsetFromTop +  j * cBrickSizeHeight + j; // TODO This is shit
            }
        }
    }

    void setup()
    {

        // Restore default values and initialize all the bricks
        this->restoreDefaultValues();

        // Show welcome screen
        GUI& gui = gui.getInstance();
        gui.welcomeScreen("ARKANOID");
    }


    void loop()
    {
        GUI& gui = GUI::getInstance();
        gui.display.clearDisplay();

        //move the player
        if (gui.isKeyPressed(Buttons::Keys::eKey_Left)) {
            playerPosX = max(0, playerPosX - playerVelocityX);
        }
        if (gui.isKeyPressed(Buttons::Keys::eKey_Right)) {
            playerPosX = min(LCDWIDTH - cPlayerWidth, playerPosX + playerVelocityX);
        }

        // Make the game faster
        if (gui.isKeyPressed(Buttons::Keys::eKey_Up)) {
            ballVelocityX = (ballVelocityX / abs(ballVelocityX)) * 2;
            ballVelocityY = (ballVelocityY / abs(ballVelocityY)) * 2;
            playerVelocityX = 3;
        }

        // Make the game slower
        if (gui.isKeyPressed(Buttons::Keys::eKey_Down)) {
            ballVelocityX = (ballVelocityX / abs(ballVelocityX)) * 1;
            ballVelocityY = (ballVelocityY / abs(ballVelocityY)) * 1;
            playerVelocityX = 2;
        }


        if (gui.isKeyPressed(Buttons::Keys::eKey_Enter)) {
            if (bIsBallMoving == false)
            {
                bIsBallMoving = true;
            }

            if (bHasFire == true)
            {
                // Fire, madafaka !
            }

            //playerPosY = min(LCDWIDTH - playerPosX, playerPosX + playerVelocityX);
            // Fire !
        }

        if (bIsBallMoving)
        {
            //move the ball
            ballPosX = ballPosX + ballVelocityX;
            ballPosY = ballPosY + ballVelocityY;
        }
        else
        {
            // Ball is on the player platform
            ballPosX = playerPosX + (cPlayerWidth / 2);
            ballPosY = playerPosY - cBallSize;
        }

        //check for ball collisions
        //collision with the top border
        if (ballPosY < 0) {
            ballPosY = 0;
            ballVelocityY = -ballVelocityY;
            //gb.sound.playTick();
        }

        //collision with the left side
        if (ballPosX <= 0) {
            ballPosX = 0;
            ballVelocityX = -ballVelocityX;
            
            //ballPosY = random(0, LCDHEIGHT - cBallSize);
        }

        //collision with the right side
        if ((ballPosX + cBallSize) >= LCDWIDTH) {
            ballPosX = LCDWIDTH - cBallSize;
            ballVelocityX = -ballVelocityX;
        }

        // Collision detection with bricks
        
        for (int i = 0; i < cBrickCount; i++)
        {   
            Brick& rBrick = bricks[i];
            if (rBrick.active)
            {
                if (gui.collideRectRect(ballPosX,
                                        ballPosY, 
                                        cBallSize,
                                        cBallSize,
                                        rBrick.x,
                                        rBrick.y,
                                        cBrickSizeWidth,
                                        cBrickSizeHeight))
                {
                    rBrick.active = false;
                    ballVelocityY = -ballVelocityY;
                }
                // Collision detection
                // Collision from side
                // Collision from top/bottom
                //gb.collideRectRect(ballPosX, ballPosY, ball_size, ball_size, playerPosX, playerPosY, cPlayerWidth, player_h)) {
            }
        }

        //collision with the player
        if (gui.collideRectRect(ballPosX, ballPosY, cBallSize, cBallSize, playerPosX, playerPosY, cPlayerWidth, cPlayerHeight))
        {
            ballPosY = playerPosY - cBallSize; // - player_h */;
            ballVelocityY = -abs(ballVelocityY);
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
        gui.display.fillRect(ballPosX, ballPosY, cBallSize, cBallSize, 1);
        //draw the player
        gui.display.fillRect(playerPosX, playerPosY, cPlayerWidth, cPlayerHeight, 1);

        // Draw the gui around the screen
        gui.display.drawFastHLine(0,0, gui.display.width(), BLACK);
        gui.display.drawFastVLine(gui.display.width() - 1, 0, gui.display.height(), BLACK);
        gui.display.drawFastVLine(0,0, gui.display.height(), BLACK);

        // Draw the bottom
        gui.drawDashedHLine(1, LCDHEIGHT - 2, gui.display.width(), 2, 2);


        //gui.display.drawPixel() // Add dashed line on the bottom


        //draw all the boxes
        for (int i = 0; i < cBrickCount; i++)
        {
            Brick& brick = bricks[i];
            if (brick.active)
            {
                gui.display.fillRect(brick.x, brick.y, cBrickSizeWidth, cBrickSizeHeight, 1);
            }
        }
        //gui.display.fillRect(oponent_x, oponent_y, oponent_w, oponent_h, 1);
        //gb.gui.display.fillRect(oponent_x, oponent_y, oponent_w, oponent_h);


        //collision with the bottom border to detect game over - is done here after everything was already shown on the screen
        if ((ballPosY + cBallSize) >= LCDHEIGHT)
        {
            // Game over
            // Restore default values and initialize all the bricks
            this->restoreDefaultValues();

            gui.showPopUp("GAME OVER");
        }



        gui.display.display();
    }
};
