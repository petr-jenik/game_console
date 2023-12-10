#include "my_gui.h"

#include <queue> // std::queue
#include <deque> // std::deque
/*
class Game():
    SIZE_X = 40
    SIZE_Y = 20
    def __init__(self):
        # Define game dimensions
        self.size_x = Game.SIZE_X
        self.size_y = Game.SIZE_Y

        self.queue = Queue()

        # Create a snake's body in the middle of the game area
        middle_point_x = self.size_x//2
        middle_point_y = self.size_y//2
        self.head_position_x = middle_point_x + 1
        self.head_position_y = middle_point_y

        self.queue.push((middle_point_x -1, middle_point_y))
        self.queue.push((middle_point_x   , middle_point_y))
        self.queue.push((middle_point_x +1, middle_point_y))

    def setDirection(self, direction):
        self.direction = direction

    def move(self):
        if self.direction == "up":
            self.head_position_y += 1
        elif self.direction == "down":
            self.head_position_y -= 1
        elif self.direction == "left":
            self.head_position_x -= 1
        elif self.direction == "right":
            self.head_position_x += 1

        self.head_position_x = (self.head_position_x + self.size_x) % self.size_x
        self.head_position_y = (self.head_position_y + self.size_y) % self.size_y

        self.queue.push((self.head_position_x, self.head_position_y))
        self.queue.pop()

    def show(self):
        
        print("*" * self.size_x)

        for y in range(self.size_y):
            row_string = ""
            for x in range(self.size_x):

                if (self.head_position_x == x) and (self.head_position_y == y):
                    row_string += "O"
                elif self.queue.contains((x,y)):
                    row_string += "#"
                else:
                    row_string += "_"
            print(f"row: {row_string}")



if __name__ == "__main__":
    game = Game()
    
    for i in range(10):
        for direction in ("right", "up", "left", "down"):
            game.setDirection(direction)
            for j in range(5 + i):
                game.move()
                game.show()
                time.sleep(0.1)
*/

class Snake : public GameAPI
{
public:
    enum Direction {
        Direction_Up = 1,
        Direction_Down,
        Direction_Left,
        Direction_Right
    };

    typedef struct Position
    {
        int32_t x;
        int32_t y;
        Direction eDirection;
    };

private:
    // Define game dimensions
    static const int32_t cGameAreaSizeX = 20;
    static const int32_t cGameAreaSizeY = 9;

    const int32_t cElementSize = 4;

    std::deque<Position> queue;
    //Queue queue;//   = Queue();
    Position mHeadPosition;
    //Direction mDirection;

    enum BodyPart
    {
        BodyPart_Head,
        BodyPart_Body,
        BodyPart_Tail
    };

    void drawPart(GUI& gui, int32_t x, int32_t y, BodyPart ePart, Direction eDirection)
    {
        //GUI& gui = GUI::getInstance();

        switch(ePart)
        {
            case BodyPart_Head:
            {
                gui.display.drawFastHLine(x, y + 2, 5, BLACK);
                gui.display.drawPixel(x + 1, y +1, BLACK);
                gui.display.drawFastHLine(x + 3, y + 1, 2, BLACK);
                gui.display.drawPixel(x + 2, y, BLACK);
                break;
            }
            case BodyPart_Body:
            {
                if (eDirection == Direction::Direction_Right)
                {
                    gui.display.drawFastHLine(x + 1, y + 1, 3, BLACK);
                    gui.display.drawFastHLine(x    , y + 2, 3, BLACK);
                }
                if (eDirection == Direction::Direction_Left)
                {
                    gui.display.drawFastHLine(x    , y + 1, 3, BLACK);
                    gui.display.drawFastHLine(x + 1, y + 2, 3, BLACK);
                }
                if (eDirection == Direction::Direction_Up)
                {
                    gui.display.drawFastVLine(x + 1, y + 1, 3, BLACK);
                    gui.display.drawFastVLine(x + 2, y    , 3, BLACK);
                }
                if (eDirection == Direction::Direction_Down)
                {
                    gui.display.drawFastVLine(x + 1, y    , 3, BLACK);
                    gui.display.drawFastVLine(x + 2, y + 1, 3, BLACK);
                }

                break;
            }
            case BodyPart_Tail:
            {
                if (eDirection == Direction::Direction_Right)
                {
                    gui.display.drawFastHLine(x    , y + 1, 4, BLACK);
                    gui.display.drawFastHLine(x - 2, y + 2, 5, BLACK);
                }
                if (eDirection == Direction::Direction_Left)
                {
                    gui.display.drawFastHLine(x    , y + 1, 4, BLACK);
                    gui.display.drawFastHLine(x +1 , y + 2, 5, BLACK);
                }
                if (eDirection == Direction::Direction_Up)
                {
                    gui.display.drawFastVLine(x + 1, y    , 4, BLACK);
                    gui.display.drawFastVLine(x + 2, y - 2, 5, BLACK);
                }
                if (eDirection == Direction::Direction_Down)
                {
                    gui.display.drawFastVLine(x + 1, y    , 4, BLACK);
                    gui.display.drawFastVLine(x + 2, y - 2, 5, BLACK);
                }
                break;
            }
            default:
                break;
        }
    }


    // Define GUI element size
    static const uint8_t cBLockSize = 4;
    
public:


    void setDirection(Direction eDirection)
    {
        //this->mDirection = eDirection;
        this->mHeadPosition.eDirection = eDirection;
    }

    Snake()
    {
        restoreDefaultValues();
    }
    //player variables

    void restoreDefaultValues()
    {
        // Empty the queue
        queue.clear();


        // Create a snake's body in the middle of the game area
        const Position cMiddlePoint = { cGameAreaSizeX / 2, cGameAreaSizeY / 2};

        this->mHeadPosition.x = cMiddlePoint.x + 1;
        this->mHeadPosition.y = cMiddlePoint.y;
        this->mHeadPosition.eDirection = Direction::Direction_Right;
        this->setDirection(Direction::Direction_Right);        

        for (int i = 5; i >= 0; i--){
            this->queue.push_front({cMiddlePoint.x - i, cMiddlePoint.y, Direction::Direction_Right});    
        }
        /*this->queue.push_front({cMiddlePoint.x -5, cMiddlePoint.y});
        this->queue.push_front({cMiddlePoint.x -4, cMiddlePoint.y});
        this->queue.push_front({cMiddlePoint.x -3, cMiddlePoint.y});
        this->queue.push_front({cMiddlePoint.x -2, cMiddlePoint.y});
        this->queue.push_front({cMiddlePoint.x -1, cMiddlePoint.y});
        this->queue.push_front({cMiddlePoint.x   , cMiddlePoint.y});*/

        this->queue.push_front({cMiddlePoint.x +1, cMiddlePoint.y, Direction::Direction_Right});
    }

    void setup()
    {
        // Restore default values and initialize all the bricks
        this->restoreDefaultValues();

        // Show welcome screen
        GUI& gui = gui.getInstance();
        gui.welcomeScreen("Snake");
    }

    void move() {
        if (this->mHeadPosition.eDirection == Direction::Direction_Up) {
            this->mHeadPosition.y -= 1;
        }
        else if (this->mHeadPosition.eDirection == Direction::Direction_Down) {
            this->mHeadPosition.y += 1;
        }
        else if (this->mHeadPosition.eDirection == Direction::Direction_Left) {
            this->mHeadPosition.x -= 1;
        }
        else if (this->mHeadPosition.eDirection == Direction::Direction_Right) {
            this->mHeadPosition.x += 1;
        }

        // Add collision detection
        // Add food detection
        // Push not only coordinates but even a direction
        // TODO Delete head position
        this->mHeadPosition.x = (this->mHeadPosition.x + Snake::cGameAreaSizeX) % Snake::cGameAreaSizeX;
        this->mHeadPosition.y = (this->mHeadPosition.y + Snake::cGameAreaSizeY) % Snake::cGameAreaSizeY;
        
        this->queue.push_front(this->mHeadPosition);
        this->queue.pop_back();
    }

    void loop()
    {
        GUI& gui = GUI::getInstance();
        gui.display.clearDisplay();

        //move the player
        if (gui.buttons.pressed(Buttons::eKey_Left)) {
            this->setDirection(Direction::Direction_Left);
        }
        if (gui.buttons.pressed(Buttons::eKey_Right)) {
            this->setDirection(Direction::Direction_Right);
        }
        if (gui.buttons.pressed(Buttons::eKey_Up)) {
            this->setDirection(Direction::Direction_Up);
        }
        if (gui.buttons.pressed(Buttons::eKey_Down)) {
            this->setDirection(Direction::Direction_Down);
        }

        // Udpdate snake's position
        this->move();

        /*if (gui.buttons.pressed(Buttons::eKey_Enter)) {
            if (this->mSourceCol == -1) {                
                this->mSourceCol = this->mSelectedCol;
            }
            else {
                move(this->mSourceCol, this->mSelectedCol);
                this->mSourceCol = -1;
            }
        }*/

        const int32_t gameAreaOffsetY = 8;
        const int32_t gameAreaOffsetX = 2;
        const int32_t gameAreaSizeX = LCDWIDTH - 4;
        const int32_t gameAreaSizeY = LCDHEIGHT - 4;
        const int32_t gameAreaWidth = LCDWIDTH - 4;

        const int32_t gameAreaHeight = LCDHEIGHT - gameAreaOffsetY;

        //draw the snake
        //const int32_t cSnakeBodyLength = this->queue.size();
        /*for (uint8_t iCol = 0; iCol < cColumnCount; iCol++)*/
        

        size_t cElementCount = this->queue.size();
        for (int32_t i = 0; i < cElementCount; i++)
        //for (auto bodyPart : this->queue)
        {
            auto bodyPart = this->queue[i];
            int16_t x = gameAreaOffsetX +  4 * bodyPart.x;
            int16_t y = gameAreaOffsetY + 4 * bodyPart.y;

            if (i == 0){
                // Draw head
                this->drawPart(gui, x, y, BodyPart::BodyPart_Head, bodyPart.eDirection);
                //gui.display.fillRect(x, y, cElementSize * 3, cElementSize * 3, BLACK);
            }
            else if (i == (cElementCount - 1)){
                // Draw tail
                this->drawPart(gui, x, y, BodyPart::BodyPart_Tail, bodyPart.eDirection);
                //gui.display.fillRect(x, y, cElementSize * 2, cElementSize * 2, BLACK);
            }
            else{
                // Draw body
                this->drawPart(gui, x, y, BodyPart::BodyPart_Body, bodyPart.eDirection);
                //gui.display.fillRect(x, y, cElementSize, cElementSize, BLACK);
            }
        }

        String score = "\x03\x03\x03";
        //#score.concat(this->mMoveCount);

        gui.alignText(score.c_str(), LCDWIDTH, 0, BLACK, GUI::eHorizontalRightAlign, GUI::eVerticalBottomAlign);
        gui.display.drawFastHLine(0, 6, LCDWIDTH, BLACK);
        gui.display.drawFastHLine(0, 8, LCDWIDTH, BLACK);
        gui.display.drawFastVLine(0, 8, LCDHEIGHT - 8, BLACK);
        gui.display.drawFastVLine(LCDWIDTH -1, 8, LCDHEIGHT - 8, BLACK);
        gui.display.drawFastHLine(0, LCDHEIGHT - 1, LCDWIDTH, BLACK);
        /*
        // Check game over condition
        if (this->isFinished()) {
            gui.showPopUp("Done!");

            // TODO Blink with the snake
        }*/

        gui.display.display();
    }
};
