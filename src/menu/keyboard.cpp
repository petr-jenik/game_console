
#ifdef BUILD_FOR_X86
#include <ArduinoAPI.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

//#include "menu_item.hpp"
#include "my_gui.h"

#include "keyboard.hpp"

const char* cShift = "^";
const char* cSymbols = "&";
const char* cBackspace = "<";
const char* cOK = "\x1C";

//const char cSpace = 'B';
//const char cSpace = 'OK';

void screen_keyboard_demo()
{
    GUI& gui = GUI::getInstance();

    gui.display.setTextWrap(true);

    int textSize = 0;
    gui.display.setTextSize(textSize);


    String tmpString = "";

    for (int i = 0; i < 256; i++)
    {
        tmpString.concat(static_cast<char>(i));
    }

    int offset = 0;
    while (1)
    {
        if (gui.update())
        {
            
            gui.display.clearDisplay();
            gui.display.setCursor(0, offset);
            gui.display.setTextColor(BLACK);

            
            //tmpString.concat("\nchar: ");
            //#tmpString.concat(character);
            // 27 - shitf
            // 30 - backspace
            // 31 - mezera 
            // & - symboly
            gui.display.print(tmpString);
            gui.display.display();
            delay(100);
            offset--;
            //character++;
        }
    }
}

void getTextBounds(const char * text, uint16_t & width, uint16_t & height)
{
    int16_t x1 = 0;
    int16_t y1 = 0;
    GUI::getInstance().display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);
}



void screen_keyboard()
 {
    GUI& gui = GUI::getInstance();

    const int cKeyboardRowCount = 4;
    const int cKeyboardColCount = 11;

    const char* keyboard[] = {
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", cBackspace,
        "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", cSymbols,
     cShift, "A", "S", "D", "F", "G", "H", "J", "K", "L", cShift,
        "Z", "X", "C", "V", "B", "N", "M", ",", ".", " ", cOK
    };

    int selectedRow = 0;
    int selectedCol = 0;

    String input_text = "";

    gui.display.setTextWrap(false);

    int textSize = 1;
    gui.display.setTextSize(textSize);

    while (1)
    {
        if (gui.update())
        {
            gui.display.clearDisplay();
            gui.display.setTextSize(textSize);

            // Draw keyboard
            for (int iRow = 0; iRow < cKeyboardRowCount; iRow++)
            {
                int lineWidth = 0;
                for (int iCol = 0; iCol < cKeyboardColCount; iCol++)
                {
                    const char* keyStr = keyboard[iRow * cKeyboardColCount + iCol];
                    // Symbols should have 0.5xsymbol width distance
                    const int rectDist = 2;

                    uint16_t keyWidth;
                    uint16_t keyHeight;
                    //gui.display.getTextBounds(" ", 0, 0, &x1, &y1, &basekeyWidth, &basekeyHeight);
                    getTextBounds(keyStr, keyWidth, keyHeight);

                    lineWidth += keyWidth;

                    std::cout << keyWidth << ", " << keyHeight << std::endl;

                    //int x = LCDWIDTH / 2 - lineWidth; //+ ((iCol - selectedCol) * (keyWidth + rectDist));
                    int x = LCDWIDTH / 2 + ((iCol - selectedCol) * (keyWidth + rectDist));
                    int y = LCDHEIGHT / 2 + ((iRow- selectedRow) * (keyHeight + rectDist));

                    //gui.display.drawRect(x, y, keyWidth, keyHeight, BLACK);
                    //gui.display.drawRect(x - 2, y - 2, keyWidth + 3+1, keyHeight + 3+1, BLACK);

                    gui.display.drawRect(x - rectDist, y - rectDist, keyWidth + 2*rectDist - 1, keyHeight + 2*rectDist - 1, BLACK);

                    gui.display.setCursor(x, y);
    

                    if (selectedRow == iRow && selectedCol == iCol)
                    {
                        gui.display.fillRect(x - rectDist, y - rectDist, keyWidth + 2*rectDist - 1, keyHeight + 2*rectDist - 1, BLACK);
                        gui.display.setTextColor(WHITE);
                        //gui.display.drawRect(x - 2, y - 2, keyWidth + 3 + 1, keyHeight + 3 + 1, BLACK);
                        //gui.display.drawRect(x - 3, y - 3, keyWidth + 3 + 3, keyHeight + 3 + 3, BLACK);
                        /*for (int i = 2; i < 5; i++)
                        {
                            //gui.display.drawRect(x - i, y - i, keyWidth + 2*i, keyHeight + 2*i, BLACK);
                        }*/
                    }
                    else
                    {
                        gui.display.setTextColor(BLACK);
    
                    }
                    // Dirty solution converting a single character to string
                    //char tmpString[2] = {key, '\0'};

                    
                    gui.display.print(keyStr);
                    gui.display.setTextColor(BLACK);
                    //gui.alignText(tmpString, x, y, BLACK, GUI::eHorizontalLeftAlign, GUI::);
                }
            }

            //gui.display.setCursor(, y);
            gui.display.setTextSize(0);
            gui.display.drawFastHLine(0, LCDHEIGHT - 10, LCDWIDTH, BLACK);
            gui.display.fillRect(0, LCDHEIGHT - 9, LCDWIDTH, 9, WHITE);
            gui.alignText(input_text.c_str(), LCDWIDTH, LCDHEIGHT, BLACK, GUI::eHorizontalRightAlign, GUI::eVerticalTopAlign);

            gui.display.display();
        
            if (gui.buttons.repeat(Buttons::eKey_Back, 1))
            {
                break;
            }

            if (gui.buttons.repeat(Buttons::eKey_Left, 10))
            {
                if (selectedCol > 0)
                {
                    selectedCol--;
                }
            }

            if (gui.buttons.repeat(Buttons::eKey_Right, 10))
            {
                if (selectedCol < cKeyboardColCount - 1)
                {
                    selectedCol++;
                }
            }

            if (gui.buttons.repeat(Buttons::eKey_Up, 10))
            {
                if (selectedRow > 0)
                {
                    selectedRow--;
                    /*
                    if (selectedCol >= cKeyboardColCount)
                    {
                        selectedCol = cKeyboardColCount - 1;
                    }*/
                }
            }

            if (gui.buttons.repeat(Buttons::eKey_Down, 10))
            {
                if (selectedRow < cKeyboardRowCount - 1)
                {
                    selectedRow++;
                    /*if (selectedCol >= strlen(keyboard[selectedRow]))
                    {
                        selectedCol = strlen(keyboard[selectedRow]) - 1;
                    }*/
                }
            }

            if (gui.buttons.repeat(Buttons::eKey_Enter, 10))
            {
                const char* keyStr = keyboard[selectedRow * cKeyboardColCount + selectedCol];
                //const char* key = &keyboard[selectedRow][selectedCol];
                //input_text.concat(*key);
                
                // Check if a special key is selected
                if ((keyStr == cShift) || (keyStr == cSymbols) || (keyStr == cBackspace) || (keyStr == cOK))
                {
                    if (keyStr == cBackspace)
                    {
                        // Remove last character
                        if (input_text.length())
                        {
                            input_text.remove(input_text.length() - 1);
                        }
                    }
                }
                else
                {
                    input_text += keyStr;
                    std::cout << "key: " << keyStr << std::endl;
                    std::cout << "selectedRow: " << selectedRow << std::endl;
                    std::cout << "selectedCol: " << selectedCol << std::endl;
                    std::cout << "Input text: " << input_text.c_str() << std::endl;
                }
                
            }
        }
    }
}



void screen_keyboard_good()
{
    GUI& gui = GUI::getInstance();

    const char* keyboard[] = {
        "1234567890",
        "qwertyuiop",
        "asdfghjkl",
        "zxcvbnm"
    };

    int selectedRow = 0;
    int selectedCol = 0;

    String input_text = "";

    while (1)
    {
        if (gui.update())
        {
            gui.display.clearDisplay();
            gui.display.setTextSize(0);

            // Draw keyboard
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < strlen(keyboard[i]); j++)
                {
                    const char key = keyboard[i][j];
                    int x = j * 8 + 1;
                    int y = i * 10 + 1;

                    if (selectedRow == i && selectedCol == j)
                    {
                        gui.display.drawRect(x - 2, y - 2, 9, 11, BLACK);
                        gui.display.drawRect(x - 3, y - 3, 11, 13, BLACK);
                    }

                    // Dirty solution converting a single character to string
                    char tmpString[2] = {key, '\0'};

                    gui.display.setCursor(x, y);
                    gui.display.setTextColor(BLACK);
                    gui.display.print(tmpString);

                    //gui.alignText(tmpString, x, y, BLACK, GUI::eHorizontalLeftAlign, GUI::);
                }
            }

            gui.display.display();
        
            if (gui.buttons.repeat(Buttons::eKey_Back, 1))
            {
                break;
            }

            if (gui.buttons.repeat(Buttons::eKey_Left, 10))
            {
                if (selectedCol > 0)
                {
                    selectedCol--;
                }
            }

            if (gui.buttons.repeat(Buttons::eKey_Right, 10))
            {
                if (selectedCol < strlen(keyboard[selectedRow]) - 1)
                {
                    selectedCol++;
                }
            }

            if (gui.buttons.repeat(Buttons::eKey_Up, 10))
            {
                if (selectedRow > 0)
                {
                    selectedRow--;
                    if (selectedCol >= strlen(keyboard[selectedRow]))
                    {
                        selectedCol = strlen(keyboard[selectedRow]) - 1;
                    }
                }
            }

            if (gui.buttons.repeat(Buttons::eKey_Down, 10))
            {
                if (selectedRow < 3)
                {
                    selectedRow++;
                    if (selectedCol >= strlen(keyboard[selectedRow]))
                    {
                        selectedCol = strlen(keyboard[selectedRow]) - 1;
                    }
                }
            }

            if (gui.buttons.repeat(Buttons::eKey_Enter, 10))
            {
                const char* key = &keyboard[selectedRow][selectedCol];
                //input_text.concat(*key);
                input_text += *key;
                std::cout << "key: " << *key << std::endl;
                std::cout << "selectedRow: " << selectedRow << std::endl;
                std::cout << "selectedCol: " << selectedCol << std::endl;
                std::cout << "Input text: " << input_text.c_str() << std::endl;
            }
        }
    }
}



void screen_keyboard_old()
{
    GUI& gui = GUI::getInstance();

    int currentCharIndex = 0;

    String inputString = "";
    int screenOffset = 0;
    while(1)
    {

        char currentChar = 'A';
        if (gui.update())
        {
            gui.display.clearDisplay();
            gui.display.setTextSize(0);
            
            // Draw the keyboard
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 12; j++)
                {
                    gui.alignText(String(currentChar).c_str(), j*6 + 3 - screenOffset, i*8 + 5, BLACK, GUI::eHorizontalLeftAlign);
                    currentChar++;
                }
            }
            
            // Draw the cursor
            gui.display.drawFastVLine(1, 5, 8, BLACK);
            gui.display.drawFastVLine(2, 5, 8, BLACK);
            
            // Draw the input string
            gui.alignText(inputString.c_str(), LCDWIDTH / 2, 25, BLACK, GUI::eHorizontalCenterAlign);
            
            // Show the display
            gui.display.display();
        }

        if (gui.buttons.repeat(Buttons::eKey_Back, 1))
        {
            break;
        }

        if (gui.buttons.repeat(Buttons::eKey_Left, 10))
        {
            if (currentCharIndex > 0)
            {
                currentCharIndex--;
            }
            if (currentCharIndex < screenOffset)
            {
                screenOffset--;
            }
        }

        if (gui.buttons.repeat(Buttons::eKey_Right, 10))
        {
            if (currentCharIndex < 59)
            {
                currentCharIndex++;
            }
            if (currentCharIndex > screenOffset + 11)
            {
                screenOffset++;
            }
        }

        if (gui.buttons.repeat(Buttons::eKey_Enter, 10))
        {
            inputString += (char)('A' + currentCharIndex);
        }
    }
}
