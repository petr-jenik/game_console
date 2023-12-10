#pragma once

#ifdef BUILD_FOR_X86
#include <iostream>
#include <ArduinoApi.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//#include "fake_display.h"
#include "Buttons.h"
//#include "my_user_input.h"
#include "Battery.hpp"

#include <cstdint>

/* Declare LCD object for SPI
 Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);*/
static const int32_t sclk_pin = 18; //
static const int32_t din_pin = 23; // 
static const int32_t dc_pin = 21; //  
static const int32_t cs_pin = 5; // 
static const int32_t rst_pin = 17; //

static const int32_t backlight_pin = 16; //

#define NOROT 0
#define ROTCCW 1
#define ROT180 2
#define ROTCW 3
#define NOFLIP 0
#define FLIPH 1
#define FLIPV 2
#define FLIPVH 3

class GUI
{
public:

    GUI()
    :display(Adafruit_PCD8544(sclk_pin, din_pin, dc_pin, cs_pin, rst_pin)) // TODO Solve hardcoded pin numbers
    {};

    static GUI& getInstance()
    {
        static GUI instance;
        return instance;
    }

    void backlightInit()
    {
        setBacklight(true);
        pinMode(backlight_pin, OUTPUT);
    }

    void setBacklight(bool bTurnOn)
    {
        // As backlight is controlled by pulling LED to the ground, an inverted logic has to be used.
        digitalWrite(backlight_pin, (bTurnOn) ? LOW : HIGH);
    }

    void init()
    {
        this->backlightInit();
        //this->display.initDisplay();
        // Rotate the display

        this->display.begin();
        this->display.setRotation(2);
        // init done

        // you can change the contrast around to adapt the display
        // for the best viewing!
        this->display.setContrast(53);
        this->display.setBias(5);

        // Initialize buttons
        this->buttons.begin();
    }


    bool update()
    {
        //this->display.display();
        // Draw the display more often, maybe it will improve picture quality on the real LCD.
        // 50 ms = 20 FPS
        // 67 ms = 15 FPS

        unsigned long endTime = millis() + 67;

        //Serial.print("End time: ");
        //Serial.println(endTime); // prints time since program started
        //std::cout << "End time: " << endTime << std::endl;


        while(millis() < endTime)
        {
            //Serial.print("Current time: ");
            //Serial.println(millis()); // prints time since program started

            //std::cout << "Current time" << millis() << std::endl;
            this->display.display();
            
            // Why not?
             delay(2);
        }

        //this->display.clearDisplay();

        buttons.update();
        //delay(67);
        return true;
        // TODO Implement here
    }

/*boolean Gamebuino::update() {
	if (((nextFrameMillis - millis()) > timePerFrame) && frameEndMicros) { //if time to render a new frame is reached and the frame end has ran once
		nextFrameMillis = millis() + timePerFrame;
		frameCount++;

		frameEndMicros = 0;
		frameStartMicros = micros();

		backlight.update();
		buttons.update();
		battery.update();

		return true;

	} else {
		if (!frameEndMicros) { //runs once at the end of the frame
		
			//increase volume shortcut : hold C + press B
			if(buttons.repeat(BTN_C, 1) && buttons.pressed(BTN_B)){
				sound.setVolume(sound.getVolume() + 1);
				popup("HELLO", 60);
				sound.playTick();
			}
			
			//flash loader shortcut : hold C + A
			if(buttons.repeat(BTN_C, 1) && buttons.pressed(BTN_A)){
				popup(F("\027+\025 \020 LOADER"), 60);
			}
			if(buttons.repeat(BTN_C, 1) && buttons.held(BTN_A,40)){
				changeGame();
			}
		
			sound.updateTrack();
			sound.updatePattern();
			sound.updateNote();
			updatePopup();
			displayBattery();
			
			display.update(); //send the buffer to the screen
			if(!display.persistence)
			display.clear(); //clear the buffer

			frameEndMicros = micros(); //measure the frame's end time
			frameDurationMicros = frameEndMicros - frameStartMicros;

			//            display.setTextColor(BLACK);
			//            display.setCursor(0, 40);
			//            display.print(frameDurationMicros / timePerFrame);
			//            display.print(" ");
			//            display.print(2048 - freeRam());

			//            display.setCursor(0, 32);
			//            display.print("CPU:");
			//            display.print(frameDurationMicros / timePerFrame);
			//            display.println("/1000");
			//            display.print("RAM:");
			//            display.print(2048 - freeRam());
			//            display.println("/2048");
		}
		return false;
	}
}

void Gamebuino::setFrameRate(uint8_t fps) {
	timePerFrame = 1000 / fps;
	sound.prescaler = fps / 20;
	sound.prescaler = max(1, sound.prescaler);
}
*/

    void drawDashedHLine(int16_t x, int16_t y, int16_t h, int16_t partLength = 1, int16_t lineWidth = 1)
    {
        if (partLength == 0)
        {
            this->display.drawFastHLine(x, y, h, BLACK);
            return;
        }
        
        for (int16_t currentX = 0; currentX < h; currentX++)
        {
            // This creates a dashed line with the same length of black and transparent parts
            int16_t tmp = currentX / partLength;
            
            // Start with a black pixel
            if ((tmp % 2) == 0)
            {
                // Add width if needed
                for (int iLineWidth = 0; iLineWidth < lineWidth; iLineWidth++)
                {
                    this->display.drawPixel(x + currentX, y + iLineWidth, BLACK);
                }
            }
        }
    }

    void drawDashedVLine(int16_t x, int16_t y, int16_t l, int16_t partLength = 1, int16_t lineWidth = 1)
    {
        if (partLength == 0)
        {
            this->display.drawFastVLine(x, y, l, BLACK);
            return;
        }

        for (int16_t currentY = 0; currentY < l; currentY++)
        {
            // This creates a dashed line with the same length of black and transparent parts
            int16_t tmp = currentY / partLength;
            
            // Start with a black pixel
            if ((tmp % 2) == 0)
            {
                // Add width if needed
                for (int iLineWidth = 0; iLineWidth < lineWidth; iLineWidth++)
                {
                    this->display.drawPixel(x + iLineWidth, y + currentY, BLACK);
                }
            }
        }
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

    bool isKeyPressed(Buttons::Keys eKey)
    {
        return this->buttons.repeat(eKey, 1);
        //return Buttons::isKeyPressed(eKey);
    }

    void welcomeScreen(String message)
    {
        this->display.clearDisplay();
        this->display.setTextSize(1);
        this->display.setTextColor(BLACK);

        this->alignText(message.c_str(), LCDWIDTH / 2, LCDHEIGHT / 2, BLACK, GUI::eHorizontalCenterAlign, GUI::eVerticalCenterAlign);
        this->display.display();

        delay(1000);
    }

    void showPopUp(String message, bool bBlocking = true)
    {
        //this->display.clearDisplay();
        this->display.setTextSize(1);
        this->display.setTextColor(BLACK);

        const int cRectOffsetFromSide = 5;

        this->display.fillRect(/* x */ cRectOffsetFromSide,
                               /* y */ cRectOffsetFromSide,
                               LCDWIDTH - 2*cRectOffsetFromSide,
                               LCDHEIGHT - 2*cRectOffsetFromSide,
                               WHITE);

        this->display.drawRect(/* x */ cRectOffsetFromSide,
                               /* y */ cRectOffsetFromSide,
                               LCDWIDTH - 2*cRectOffsetFromSide,
                               LCDHEIGHT - 2*cRectOffsetFromSide,
                               BLACK);

        this->alignText(message.c_str(), LCDWIDTH / 2, LCDHEIGHT / 2, BLACK, GUI::eHorizontalCenterAlign, GUI::eVerticalCenterAlign);
        this->display.display();

        if (bBlocking)
        {
            delay(100);
            while(this->isKeyPressed(Buttons::eKey_Enter) == false && this->isKeyPressed(Buttons::eKey_Back) == false)
            {
                this->update();
            }
            delay(100);
        }
    }

    int8_t currentColor = BLACK;
    int8_t currentBgColor = BLACK;

    void setColor(int8_t c){
        currentColor = c;
        currentBgColor = c;
        this->display.setTextColor(c);
    }

    void setColor(int8_t c, int8_t bg){
        currentColor = c;
        currentBgColor = bg;
        this->display.setTextColor(c, bg);
    }

    // Copied from Gamebuino
    bool getBitmapPixel(const uint8_t* bitmap, uint8_t x, uint8_t y)
    {
        return pgm_read_byte(bitmap+2 + y * ((pgm_read_byte(bitmap)+7)/8) + (x >> 3)) & (B10000000 >> (x % 8));
    }

    void drawBitmap(int8_t x, int8_t y, const uint8_t *bitmap)
    {
        int8_t w = pgm_read_byte(bitmap);
        int8_t h = pgm_read_byte(bitmap + 1);
        bitmap = bitmap + 2; //add an offset to the pointer to start after the width and height
        /*this->display.drawBitmap(x, y, bitmap, w, h, BLACK)
                              int16_t w, int16_t h, uint16_t color) {*/
        int8_t i, j, byteWidth = (w + 7) / 8;
        for (j = 0; j < h; j++)
        {
            for (i = 0; i < w; i++)
            {
                if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (B10000000 >> (i % 8)))
                {
                    this->display.drawPixel(x + i, y + j, currentColor);
                }
            }
        }
    }

    void drawBitmap(int8_t x,
                    int8_t y,
                    const uint8_t *bitmap,
                    uint8_t rotation,
                    uint8_t flip)
    {
        if((rotation == NOROT) && (flip == NOFLIP))
        {
            drawBitmap(x,y,bitmap); //use the faster algorithm
            return;
        }
    }

    /*
    void drawTextOnLine(int lineIndex, String& lineString)
    {
        display.setCursor(2, i * 10);
        display.print(lineString);
    }*/

    Adafruit_PCD8544 display;
    Buttons buttons;
    Battery battery;

    enum HorizontalAlignment
    {
        eHorizontalLeftAlign,
        eHorizontalCenterAlign,
        eHorizontalRightAlign
    };

    enum VerticalAlignment
    {
        eVerticalTopAlign,
        eVerticalCenterAlign,
        eVerticalBottomAlign
    };

    void alignText(const char * pText,
                int32_t x,
                int32_t y,
                int32_t color,
                HorizontalAlignment eAlignHorizontal = eHorizontalLeftAlign,
                VerticalAlignment eAlignVertical = eVerticalCenterAlign)
    {
        int16_t upperLeftX, upperLeftY;
        uint16_t w,h;
        this->display.getTextBounds(pText, 0, 0, &upperLeftX, &upperLeftY, &w, &h);
        //std::cout << w << ", " << h << std::endl;

        //std::cout << (x - (w/2)) << ", " << y - (h/2) << std::endl;

        int alignedX = x;
        switch(eAlignHorizontal)
        {
            case eHorizontalLeftAlign:
                alignedX = x;
                break;
            case eHorizontalCenterAlign:
                alignedX = x - (w / 2);
                break;
            case eHorizontalRightAlign:
                alignedX = x - w;
                break;
            default:
                break;
        }


        int alignedY = y;
        switch(eAlignVertical)
        {
            case eVerticalTopAlign:
                alignedY = y - h;
                break;
            case eVerticalCenterAlign:
                alignedY = y - (h / 2);
                break;
            case eVerticalBottomAlign:
                alignedY = y;
                break;
            default:
                break;
        }

        //this->setColor(color);

        this->display.setCursor(alignedX, alignedY);
        this->display.println(pText);
    }
};
