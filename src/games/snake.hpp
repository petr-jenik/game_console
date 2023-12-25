#include <queue> // std::queue
#include <deque> // std::deque
#include "my_gui.h"

class Snake : public GameAPI
{
private:
    // Define game dimensions
    static const int32_t cGameAreaSizeX = 20;
    static const int32_t cGameAreaSizeY = 9;

    // Define GUI element size
    static const int32_t cElementSize = 4;
    static const int32_t cGameAreaOffsetY = 10;
    static const int32_t cGameAreaOffsetX = 2;

    /* 
     * @brief Enumaration of all posible movement directions
     */
    enum Direction {
        Direction_Right,
        Direction_Up,
        Direction_Left,
        Direction_Down,
        Direction_COUNT,
        DirectionExtension_ManuallyRotate
    };

    /*
     * @brief Position of any body part on the game area.
     */
    typedef struct 
    {
        int32_t x;
        int32_t y;
        Direction eDirection;
        bool bWasFood;
        Position() {
            this->bWasFood = false;
        }
    } Position;

    // One row of a bitmap
    typedef struct
    {
        uint8_t cols[cElementSize];
    } Row;

    typedef struct
    {
        Row rows[cElementSize];
    } SnakeBitmap;
  
    // Enumeration of all known bitmaps
    enum ElementType
    {
        ElementType_Head = 0,
        ElementType_HeadMouthOpen,
        ElementType_Body,
        ElementType_BodyWithFood,
        ElementType_Tail,
        ElementType_Corner,
        ElementType_CornerWithFood,
        ElementType_Food,
        ElementType_LAST = ElementType_Food,
        ElementType_COUNT
    };

    // Cycle counter is used to slow snake's movement when a lower difficulty level is required
    int32_t mCycleCounter;

    // Queue is a FIFO data buffer that holds all points of snake's body.
    // Deque was used to be able to access individual elements by the corresponding index.
    std::deque<Position> queue;

    // Current direction of snake's movement
    Direction mDirection;

    // Flag whether game is over or not
    bool bGameOver;

    // Store time when game ended. Needed for game over animation.
    unsigned long mGameOverTime;

    // Food object represents a food item located on a random location. Direction field can be ignored.
    Position mFood;

    /* 
     * @brief Rotate bitmap counter-clockwise by 90 degrees
     */
    void rotateBitmapLeft(SnakeBitmap &bitmap)
    {
        SnakeBitmap originalBitmap = bitmap;

        for (int iRow = 0; iRow < cElementSize; iRow++)
        {
            for (int iCol = 0; iCol < cElementSize; iCol++)
            {
                bitmap.rows[3 - iCol].cols[iRow] = originalBitmap.rows[iRow].cols[iCol];
            }
        }
    }

    /*
     * @brief Mirror bitmap horizontaly
    */
    void mirrorBitmap(SnakeBitmap &bitmap)
    {
        SnakeBitmap originalBitmap = bitmap;

        for (int iRow = 0; iRow < cElementSize; iRow++)
        {
            for (int iCol = 0; iCol < cElementSize; iCol++)
            {
                bitmap.rows[iRow].cols[3 - iCol] = originalBitmap.rows[iRow].cols[iCol];
            }
        }
    }

    /*
     * @brief Draw a bitmap to a position (x,y) with selected rotation.
     * Position is not a location on the screen, but actually a position on in a game area.
     */
    void drawPart(GUI& gui, int32_t x, int32_t y, ElementType eType, Direction eDirection, int32_t rotateN = 0)
    {
        static const Snake::SnakeBitmap bitmaps[ElementType_COUNT] = {
            {{   //Head
                {1,0,0,0},
                {0,1,1,0},
                {1,1,1,0},
                {0,0,0,0}
            }},
            {{   //Head - mouth open
                {1,0,1,0},
                {0,1,0,0},
                {1,1,0,0},
                {0,0,1,0}
            }},
            {{   //Body
                {0,0,0,0},
                {1,1,0,1},
                {1,0,1,1},
                {0,0,0,0}
            }},
            {{   //Body with food
                {0,1,1,0},
                {1,1,0,1},
                {1,0,1,1},
                {0,1,1,0}
            }},
            {{   //Tail
                {0,0,0,0},
                {0,0,1,1},
                {1,1,1,1},
                {0,0,0,0}
            }},
            {{   //Corner
                {0,1,1,0},
                {1,0,1,0},
                {1,1,0,0},
                {0,0,0,0}
            }},
            {{   //Corner with food
                {1,1,1,0},
                {1,0,1,0},
                {1,1,0,0},
                {0,0,0,0}
            }},
            {{   //Food
                {0,1,0,0},
                {1,0,1,0},
                {0,1,0,0},
                {0,0,0,0}
            }}
        };

        SnakeBitmap bitmap = bitmaps[eType];

        if (eDirection == Direction::Direction_Up)
        {
            // Rotate by 90 degree
            rotateBitmapLeft(bitmap);
        }
        if (eDirection == Direction::Direction_Left)
        {
            mirrorBitmap(bitmap);
        }
        if (eDirection == Direction::Direction_Down)
        {
            mirrorBitmap(bitmap);
            // Rotate by 90 degree
            rotateBitmapLeft(bitmap);
        }
        if (eDirection == Direction::DirectionExtension_ManuallyRotate)
        {
            // Rotate by 90 degree n-times
            for (int i = 0; i < rotateN; i++)
            {
                rotateBitmapLeft(bitmap);
            }
        }

        // Draw bitmap pixel by pixel
        for (int iRow = 0; iRow < cElementSize; iRow++)
        {
            for (int iCol = 0; iCol < cElementSize; iCol++)
            {
                if (bitmap.rows[iRow].cols[iCol] != 0)
                {
                    // convert game area coordinates (x, y) to actual screen location
                    gui.display.drawPixel(cGameAreaOffsetX + cElementSize * x + iCol,
                                          cGameAreaOffsetY + cElementSize * y + iRow,
                                          BLACK);    
                }
            }
        }
    }

    /*
     * @brief Draw snake to the game area 
     */
    void drawSnake(GUI& gui)
    {
        //draw the snake
        Direction currentDirection = this->getCurrentDirection();
        size_t cElementCount = this->queue.size();

        // Draw head and body (not tail)
        for (int32_t i = 0; i < cElementCount - 1; i++)
        {
            bool bCornerDetected = false;
            auto bodyPart = this->queue[i];
            ElementType elementType = (bodyPart.bWasFood) ? ElementType_BodyWithFood : ElementType_Body;

            // Draw head
            if (i == 0)
            {
                // This body part is the head of the snake
                auto tmpHead = bodyPart;

                // Check if the head will reach the food in next move. If yes, draw snake with open mouth.
                predictNextStep(tmpHead, tmpHead.eDirection);
                if ((tmpHead.x == this->mFood.x) && (tmpHead.y == this->mFood.y)) {
                    elementType = ElementType_HeadMouthOpen;
                }
                else {
                    elementType = ElementType_Head;
                }
            }

            if (currentDirection != bodyPart.eDirection)
            {
                //std::cout << "**** CORNER DETECTED ****" << std::endl;
                bCornerDetected = true;
            }
            if (bCornerDetected == false)
            {
                this->drawPart(gui, bodyPart.x, bodyPart.y, elementType, bodyPart.eDirection);
            }
            else
            {
                /*
                Calculate distance between current expected direction of the snake (direction of a previous part) and
                the actual direction saved in the body position object.
                If the snake turns left from its point of view(POV), value should be 1.
                When going to the right (from its POV) result would be 3.
                
                E.g. going to the right(0) and then up(1). It is Expected direction (1, going from head to tail) - actual direction stored in the data boject (0). Result is 1.
                Then we need to rotare a corner bitmap accordingly. 
                */
                auto directionDiff = (currentDirection - bodyPart.eDirection + Direction_COUNT) % Direction_COUNT;
                int32_t nRotate = 0;

                // Correct rotation of a corner part need to be calculated
                // HARD TO EXPLAIN - TODO Add comment                
                nRotate = static_cast<int32_t>(bodyPart.eDirection);
                if (directionDiff == 3)
                {
                    nRotate = (nRotate + 1) % 4;
                }

                elementType = (bodyPart.bWasFood) ? ElementType_CornerWithFood : ElementType_Corner;

                //std::cout << "directionDiff: " << directionDiff << std::endl;
                //std::cout << "rotate N-times: " << nRotate << std::endl;
                this->drawPart(gui, bodyPart.x, bodyPart.y, elementType, DirectionExtension_ManuallyRotate, nRotate);
            }

            // Configure expected direction of following parts
            currentDirection = bodyPart.eDirection;
        }
        // Draw tail
        auto lastBodyPart = this->queue[cElementCount - 1];
        // Tail needs to be rotated in a same way as previous body part
        this->drawPart(gui, lastBodyPart.x, lastBodyPart.y, ElementType_Tail, currentDirection);
    }

    /*
     * @brief Get a current direction of the snake
     */
    Direction getCurrentDirection()
    {
        return this->queue.front().eDirection;
    }

    /*
     * @brief Set a current direction of the snake
     */
    void setDirection(Direction eDirection)
    {
        this->mDirection = eDirection;
    }

    void restoreDefaultValues()
    {
        // Empty the queue
        queue.clear();

        this->bGameOver = false;

        this->mCycleCounter = 0;

        // Create a snake's body in the middle of the game area
        const Position cMiddlePoint = { cGameAreaSizeX / 2, cGameAreaSizeY / 2};

        this->setDirection(Direction::Direction_Right);        

        for (int i = 5; i >= 0; i--) {
            this->queue.push_front({cMiddlePoint.x - i, cMiddlePoint.y, Direction::Direction_Right});    
        }

        this->placeNewFood();

    }

    void predictNextStep(Position & currentPosition, Direction direction)
    {
        if (direction == Direction::Direction_Up) {
            currentPosition.y -= 1;
        }
        else if (direction == Direction::Direction_Down) {
            currentPosition.y += 1;
        }
        else if (direction == Direction::Direction_Left) {
            currentPosition.x -= 1;
        }
        else if (direction == Direction::Direction_Right) {
            currentPosition.x += 1;
        }
        // Position wrap
        currentPosition.x = (currentPosition.x + Snake::cGameAreaSizeX) % Snake::cGameAreaSizeX;
        currentPosition.y = (currentPosition.y + Snake::cGameAreaSizeY) % Snake::cGameAreaSizeY;
    }

    void move() {
        Position tmpHeadPosition = this->queue.front();

        // Find next position of the head
        predictNextStep(tmpHeadPosition, this->mDirection);
        tmpHeadPosition.eDirection = this->mDirection;
/*
        if (this->mDirection == Direction::Direction_Up) {
            tmpHeadPosition.y -= 1;
        }
        else if (this->mDirection == Direction::Direction_Down) {
            tmpHeadPosition.y += 1;
        }
        else if (this->mDirection == Direction::Direction_Left) {
            tmpHeadPosition.x -= 1;
        }
        else if (this->mDirection == Direction::Direction_Right) {
            tmpHeadPosition.x += 1;
        }
        // Position wrap
        tmpHeadPosition.x = (tmpHeadPosition.x + Snake::cGameAreaSizeX) % Snake::cGameAreaSizeX;
        tmpHeadPosition.y = (tmpHeadPosition.y + Snake::cGameAreaSizeY) % Snake::cGameAreaSizeY;
*/

        // Collision detection
        if (isPointAlreadyUsed(tmpHeadPosition.x, tmpHeadPosition.y))
        {
            this->bGameOver = true;
            this->mGameOverTime = millis();
            return;
        }

        // Food collision detection
        bool bFoodDetected = (tmpHeadPosition.x == this->mFood.x) && (tmpHeadPosition.y == this->mFood.y);
        tmpHeadPosition.bWasFood = bFoodDetected;

        // Add new position to the queue
        this->queue.push_front(tmpHeadPosition);

        if (bFoodDetected == false)
        {
            // Food not found, let's remove last element from the queue
            this->queue.pop_back();
        }
        else
        {
            // Generate new random position for food
            this->placeNewFood();
        }
    }

    bool isPointAlreadyUsed(int32_t x, int32_t y)
    {
        bool bRetval = false;
        // Collision detection
        for (auto bodyPart : this->queue)
        {
            if ((bodyPart.x == x) && (bodyPart.y == y))
            {
               bRetval = true;
            }
        }
        return bRetval;
    }

    void placeNewFood() {
        bool bFound = false;
        while(bFound == false) {
            // Find a good location for new food. It can't be place where the snake is
            long randomNumber = random(cGameAreaSizeX * cGameAreaSizeY);
            this->mFood.x = randomNumber % cGameAreaSizeX;
            this->mFood.y = randomNumber / cGameAreaSizeX;
            if (isPointAlreadyUsed(this->mFood.x, this->mFood.y)) {
                continue;
            }
            else {
                bFound = true;
            }
        }
    }

public:
    Snake()
    {
        restoreDefaultValues();
    }

    void setup()
    {
        // Restore default values and initialize all the bricks
        this->restoreDefaultValues();

        // Show welcome screen
        GUI& gui = gui.getInstance();
        gui.welcomeScreen("Snake");
    }

    void loop()
    {
        GUI& gui = GUI::getInstance();
        gui.display.clearDisplay();

        //move the player
        if (gui.buttons.pressed(Buttons::eKey_Left)) {
            if (this->getCurrentDirection() != Direction::Direction_Right) {
                this->setDirection(Direction::Direction_Left);
            }
        }
        if (gui.buttons.pressed(Buttons::eKey_Right)) {
            if (this->getCurrentDirection() != Direction::Direction_Left) {
                this->setDirection(Direction::Direction_Right);
            }
        }
        if (gui.buttons.pressed(Buttons::eKey_Up)) {
            if (this->getCurrentDirection() != Direction::Direction_Down) {
                this->setDirection(Direction::Direction_Up);
            }
        }
        if (gui.buttons.pressed(Buttons::eKey_Down)) {
            if (this->getCurrentDirection() != Direction::Direction_Up) {
                this->setDirection(Direction::Direction_Down);
            }
        }

        // Udpdate snake's position
        if ((this->bGameOver == false) && (this->mCycleCounter % 2) == 0)
        {
            this->move();
        }

        if (this->bGameOver == false) {
            // Draw snake  
            this->drawSnake(gui);
        }

        // Draw food
        this->drawPart(gui, this->mFood.x, this->mFood.y, ElementType::ElementType_Food, Direction::Direction_Right);

        //String score = "\x03\x03\x03";
        //#score.concat(this->mMoveCount);

        //gui.alignText(score.c_str(), LCDWIDTH, 0, BLACK, GUI::eHorizontalRightAlign, GUI::eVerticalBottomAlign);
        gui.display.drawFastHLine(0, 6, LCDWIDTH, BLACK);
        gui.display.drawFastHLine(0, 8, LCDWIDTH, BLACK);
        gui.display.drawFastVLine(0, 8, LCDHEIGHT - 8, BLACK);
        gui.display.drawFastVLine(LCDWIDTH -1, 8, LCDHEIGHT - 8, BLACK);
        gui.display.drawFastHLine(0, LCDHEIGHT - 1, LCDWIDTH, BLACK);

        // Check game over condition
        if (this->bGameOver) {
            // Snake should be blinking when game is over
            if (this->mCycleCounter % 8 < 4)
            {
                drawSnake(gui);
            }
            
            if ((this->mGameOverTime + 3000) < millis())
            {
                gui.showPopUp("Game over");
                this->restoreDefaultValues();
            }

            if (gui.buttons.pressed(Buttons::eKey_Enter)) {
                this->restoreDefaultValues();
            }
        }

        // Increase cycle counter
        this->mCycleCounter++;

        gui.display.display();
    }
};
