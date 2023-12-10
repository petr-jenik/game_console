#include "my_gui.h"

/*
class Game():
    def __init__(self):
        # Define game dimensions
        self.col_count = 3
        self.row_count = 10
        
        # Initialize move counter
        self.moveCounter = 0

        # Initialize buffer with game states
        tmp = [0] * self.row_count
        self.cols = []
        for i in range(self.col_count):
            self.cols.append(tmp[:])

        # Add disks to the first row
        for i in range(self.row_count):
            self.cols[0][i] =(i + 1)

    def showStats(self):
        print(f"Number of moves: {self.moveCounter}")

    def popDisc(self, iCol):
        srcIndex = self._find_firts_non_empty_index(iCol)
        if srcIndex != None:
            tmpDisc = self.cols[iCol][srcIndex]
            self.cols[iCol][srcIndex] = 0
            return tmpDisc
        else:
            return None

    def pushDisc(self, iCol, discSize):
        dstIndex = self._find_last_empty_index(iCol)
        #print(f"Pushing {discSize} to index {dstIndex}")
        if (dstIndex != None):
            self.cols[iCol][dstIndex] = discSize

    def move(self, src_col, dest_col):
        discSize = self.popDisc(src_col)
        #print("**********************")
        #print(f"discSize {discSize}")
        lastDestDisc = self.popDisc(dest_col)

        #print(f"lastDestDisc {lastDestDisc}")

        if lastDestDisc:
            self.pushDisc(dest_col, lastDestDisc)

        if (discSize != None) and ((lastDestDisc == None) or (discSize < lastDestDisc)):
            self.pushDisc(dest_col, discSize)
            self.moveCounter += 1
        elif (discSize != None):
            self.pushDisc(src_col, discSize)

    def _find_firts_non_empty_index(self, col_index):
        for disc_index, disc_size in enumerate(self.cols[col_index]):
            #print(disc_index, disc_size)
            if disc_size != 0:
                return disc_index
        return None

    # TODO TEST THIS METHOD
    def _find_last_empty_index(self, col_index):
        for disc_index, disc_size in enumerate(self.cols[col_index]):
            #print(disc_index, disc_size)
            if disc_size != 0:
                if disc_index == 0:
                    return None
                else:
                    return disc_index - 1
        return disc_index


    def show(self):
        for row in range(self.row_count):
            row_string = ""
            for col in range(self.col_count):
                disc_size = self.cols[col][row]
                col_width = 10
                row_string += ((col_width - disc_size) + 1 )//2 * "_"
                row_string += (disc_size * "#")
                row_string += (col_width - disc_size)//2 * "_"

            print(f"row{row}: {row_string}")
            #print(self.cols)


def moveit(game, frm, to):
   print (f'Presun {frm} --> {to}')
   game.move(frm, to)
   game.show()
   

def dohanoi(game, n, to, frm, using):
   if n == 0: return []
   dohanoi(game, n-1, using, frm, to);
   moveit(game, frm, to);
   dohanoi(game, n-1, to, using, frm);


*/

class Towers : public GameAPI
{
private:
    // Define game dimensions
    static const uint8_t cColumnCount = 3;
    static const uint8_t cRowCount = 5;
    
    int32_t mSourceCol;

    // Define GUI element size
    static const uint8_t cBrickSizeHeight = 5;
    
    uint8_t mSelectedCol;
    int32_t mMoveCount;
    typedef int32_t discSize_t;
    typedef discSize_t column_t[cRowCount];

    static const discSize_t cDiscSize_INVALID = -1;

    column_t mColumns[cColumnCount];
public:

    Towers()
    {
        restoreDefaultValues();
    }
    //player variables

    void restoreDefaultValues()
    {
        // Initialize move counter
        this->mMoveCount = 0;

        this->mSelectedCol = 0;
        this->mSourceCol = -1;

        // Initialize buffer with game states
        for (uint8_t iCol = 0; iCol < cColumnCount; iCol++) {
            for (uint8_t iRow = 0; iRow < cRowCount; iRow++) {
                // Add disks to the first column
                if (iCol == 0){
                    this->mColumns[iCol][iRow] = iRow + 1;
                }else{
                    this->mColumns[iCol][iRow] = 0;
                }
            }
        }
    }

    void setup()
    {

        // Restore default values and initialize all the bricks
        this->restoreDefaultValues();

        // Show welcome screen
        GUI& gui = gui.getInstance();
        gui.welcomeScreen("Tower of Hanoi");
    }

    // Get top disc from a selected column
    discSize_t popDisc(uint8_t iCol)
    {
        int32_t srcIndex = this->_find_firts_non_empty_index(iCol);
        if (srcIndex != -1) {
            discSize_t tmpDisc = this->mColumns[iCol][srcIndex];
            this->mColumns[iCol][srcIndex] = 0;
            return tmpDisc;
        }
        else
        {
            return cDiscSize_INVALID;
        }
    }

    void pushDisc(uint8_t iCol, discSize_t discSize){
        auto dstIndex = this->_find_last_empty_index(iCol);
        //#print(f"Pushing {discSize} to index {dstIndex}")
        if (dstIndex != -1)
        {
             this->mColumns[iCol][dstIndex] = discSize;
        }
    }

    int32_t _find_firts_non_empty_index(uint8_t iCol) {
        for (int32_t iRow = 0; iRow < cRowCount; iRow++) {
            auto discSize = this->mColumns[iCol][iRow];
            if (discSize != 0) {
                return iRow;
            }
        }
        return -1;
    }

    // TODO TEST THIS METHOD
    int32_t _find_last_empty_index(uint8_t iCol) {
        for (int32_t iRow = 0; iRow < cRowCount; iRow++) {
            auto discSize = this->mColumns[iCol][iRow];

            // Non-empty position found on index iRow
            // If iRow is 0 it means that the column is full -> Return -1
            // Otherwise return previous index, which should be index of last empty space
            if (discSize != 0) {
                return (iRow == 0) ? -1 : iRow - 1;
            }
        }
        // All position are empty. Last index is therefore "number of items" - 1.
        return cRowCount - 1;
    }

    void move(uint8_t iSrcCol, uint8_t iDestCol){

        // Can't move to itself
        if (iSrcCol == iDestCol){
            return;
        }

        // Check size of source disc
        auto discSize = this->popDisc(iSrcCol);

    
        // Check size of the top disc ini destination column
        auto topDestColDiscSize = this->popDisc(iDestCol);
        //#print("**********************")
        //#print(f"discSize {discSize}")
        

        //#print(f"lastDestDisc {lastDestDisc}")
        // Push top disc back to the destination column, if applicable
        if (topDestColDiscSize != cDiscSize_INVALID){
            this->pushDisc(iDestCol, topDestColDiscSize);
        }

        if ((discSize != cDiscSize_INVALID) &&
            ((topDestColDiscSize == cDiscSize_INVALID) || 
            (discSize < topDestColDiscSize)))
        {
            // Store selected disc into the destination column
            this->pushDisc(iDestCol, discSize);
            this->mMoveCount++;
        }
        else if (discSize != cDiscSize_INVALID)
        {
            // If the move was invalid, we have to return selected disc to the source column
            this->pushDisc(iSrcCol, discSize);
        }
    }      

    bool isFinished(){
        bool retval = true;

        for (int32_t iRow = 0; iRow < cRowCount; iRow++) {
            // Check values in the last column
            if (this->mColumns[this->cColumnCount - 1][iRow] != (iRow + 1))
            {
                retval = false;
                break;
            }
        }
        return retval;
    }

    void loop()
    {
        GUI& gui = GUI::getInstance();
        gui.display.clearDisplay();

        //move the player

        if (gui.buttons.pressed(Buttons::eKey_Left)) {
            this->mSelectedCol = (this->mSelectedCol + cColumnCount - 1) % cColumnCount;
        }
        if (gui.buttons.pressed(Buttons::eKey_Right)) {
            this->mSelectedCol = (this->mSelectedCol + 1) % cColumnCount;
        }

        if (gui.buttons.pressed(Buttons::eKey_Enter)) {
            if (this->mSourceCol == -1) {                
                this->mSourceCol = this->mSelectedCol;
            }
            else {
                move(this->mSourceCol, this->mSelectedCol);
                this->mSourceCol = -1;
            }
        }

        // Draw the gui around the screen
        //gui.display.drawFastHLine(0,0, gui.display.width(), BLACK);
        //gui.display.drawFastVLine(gui.display.width() - 1, 0, gui.display.height(), BLACK);
        //gui.display.drawFastVLine(0,0, gui.display.height(), BLACK);

        // Draw the bottom
        //gui.drawDashedHLine(1, LCDHEIGHT - 2, gui.display.width(), 2, 2);


        //gui.display.drawPixel() // Add dashed line on the bottom

        const int32_t gameAreaWidth = LCDWIDTH;
        const int32_t gameAreaOffsetY = 10;
        const int32_t gameAreaHeight = LCDHEIGHT - gameAreaOffsetY;


        //draw the towers
        for (uint8_t iCol = 0; iCol < cColumnCount; iCol++)
        {
            int16_t x = ((iCol + 1) * gameAreaWidth / (cColumnCount + 1));

            // Draw empty sticks
            gui.display.fillRect(x, gameAreaOffsetY, 1, gameAreaHeight, BLACK);

            for (uint8_t iRow = 0; iRow < cRowCount; iRow++)
            {
                // Draw the "discs"
                auto val = 2 * mColumns[iCol][iRow];
                if (val)
                {
                    // WTF?
                    int16_t y = gameAreaOffsetY + ((iRow + 1) * (gameAreaHeight + (cRowCount + 1)/2) / (cRowCount + 1)) - 1;
                    gui.display.fillRect(x - val, y, 1 + val*2, cBrickSizeHeight, BLACK);
                }
            }
        }


        const int32_t cSliderWidth = gameAreaWidth / 4;

        // Indicate which columns is selected
        int16_t selectedColStartX = ((this->mSelectedCol + 1) * gameAreaWidth / (cColumnCount + 1));
        gui.display.drawFastHLine(selectedColStartX - cSliderWidth/2, gameAreaOffsetY, cSliderWidth, BLACK);
        //gui.display.fillRect(selectedColStartX - cSliderWidth/2, gameAreaOffsetY, 1, cSliderWidth, BLACK);

        if (this->mSourceCol != -1) {
            int16_t sourceColStartX = ((this->mSourceCol + 1) * gameAreaWidth / (cColumnCount + 1));
            gui.drawDashedHLine(sourceColStartX - cSliderWidth/2, gameAreaOffsetY + 1, cSliderWidth);
        }
        //int16_t selectedColEndX = ((this->mSelectedCol + 2) * gameAreaWidth / (cColumnCount + 1));


        
        String score = "moves: ";
        score.concat(this->mMoveCount);

        gui.alignText(score.c_str(), LCDWIDTH, 0, BLACK, GUI::eHorizontalRightAlign, GUI::eVerticalBottomAlign);

        // Check game over condition
        if (this->isFinished()) {
            gui.showPopUp("Done!");
        }

        gui.display.display();
    }
};
