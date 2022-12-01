#pragma once

#include <iostream>
#include <string.h>
#include <zmq.h>
#include <string>
//#include <stdio.h>
#include <unistd.h>
#include <cstdint>
#include "lcd_demo.h"
#include "ArduinoApi.h"

#include "my_user_input.h"

class FakeDisplay
{
public:

    // Singleton
    static FakeDisplay* getInstance()
    {
        static FakeDisplay instance;
        return &instance;
    }

    FakeDisplay()
    :context(nullptr),
    requester(nullptr)
    {
        init();
    }

    void init()
    {
        std::cout << "Connecting to hello world server..." << std::endl;
        this->context = zmq_ctx_new ();
        this->requester = zmq_socket (this->context, ZMQ_REQ);
        zmq_connect (this->requester, "tcp://localhost:5555");

        this->sendConfig();
    }

    ~FakeDisplay()
    {
        zmq_close (this->requester);
        zmq_ctx_destroy (this->context);
    }

/*
    static FakeDisplay& getInstance()
    {
        static FakeDisplay instance;
        return instance;
    }
*/

    void sendConfig(){
        char rxBuffer[100] = {0};
        char txBuffer[100] = {0};
        sprintf(txBuffer, "%d,%d", this->LCD_WIDTH, this->LCD_HEIGHT);
        this->sendStrMessage(txBuffer);    
        this->receiveMessage(rxBuffer, sizeof(rxBuffer));
    }

    void sendStrMessage(const std::string cMsg)
    {
        //printf ("Sending message: %s\n", cMsg);
        //fflush(stdout);
        zmq_send (this->requester, cMsg.c_str(), cMsg.length(), 0);
    }

    void sendRawMessage(const void * cBuffer, const size_t cBufferSize)
    {
        zmq_send (this->requester, cBuffer, cBufferSize, 0);
    }


    void receiveMessage(char * pBuffer, size_t bufferSize)
    {
        zmq_recv (this->requester, pBuffer, bufferSize, 0);
        //printf ("Received message: %s\n", pBuffer);
        //fflush(stdout);
        // The only message which can be received (so far) is state of pressed 1s
        UserInput::processKeyStates(pBuffer);
    }

    void updateDisplay(const uint8_t * pBuffer, size_t bufferSize)
    {
        char rxBuffer[1000] = {0};
        //sprintf(buffer, "%d, %d, %d", x, y, isSet);
        
        //this->sendStrMessage(buffer);
        this->sendRawMessage(pBuffer, bufferSize);
        this->receiveMessage(rxBuffer, sizeof(rxBuffer));
    }
/*
    void setPixel(int x, int y, bool isSet)
    {
        //char buffer[1000];
        //s1f(buffer, "%d, %d, %d", x, y, isSet);
        
        if (((x >= 0) && (x < LCD_WIDTH)) && ((y >= 0) && (y < LCD_HEIGHT)))
        {
            this->displayBuffer[x][y] = isSet;
        }
        //this->sendStrMessage(buffer);
        //this->receiveMessage(buffer, sizeof(buffer));
    }
*/
    void terminate(){
        char rxBuffer[100];
        this->sendStrMessage("end");
        this->receiveMessage(rxBuffer, sizeof(rxBuffer));
    }



private:
    static const int LCD_WIDTH = 84;
    static const int LCD_HEIGHT = 48;

    void *context;
    void *requester;
    //char displayBuffer[LCD_WIDTH][LCD_HEIGHT];
    //static const size_t cSizeof_displayBuffer = sizeof(displayBuffer);

};
