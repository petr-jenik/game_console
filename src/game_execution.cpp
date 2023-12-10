#include "games.h"

#ifdef BUILD_FOR_X86
#include <ArduinoAPI.h>
#else
#include <Arduino.h>
#endif //BUILD_FOR_X86

#include "pong.hpp"
#include "Maze.hpp"
#include "Arkanoid.hpp"
#include "gameAPI.h"
#include "towers.hpp"

//#include "Super-crate-buino.hpp"

static void _playGame(void (*pSetup)(void), void (*pLoop)(void))
{
    pSetup();
    bool bRun = true;
    while(bRun)
    {
        GUI& gui = GUI::getInstance();
        if (gui.update())
        {
            // Update the game loop
            pLoop();
    
            if (gui.buttons.pressed(Buttons::eKey_Back))
            {
                // End current game
                bRun = false;
            }
        }
    }
}

static void _playGame(GameAPI& pGame)
{
    pGame.setup();
    bool bRun = true;
    while(bRun)
    {
        GUI& gui = GUI::getInstance();
        if (gui.update())
        {
            // Update the game loop
            pGame.loop();
    
            if (gui.buttons.pressed(Buttons::eKey_Back))
            {
                // End current game
                bRun = false;
            }
        }
    }
}


void pong_play()
{
    Pong pong;
    _playGame(pong);
}

void arkanoid_play()
{
    Arkanoid arkanoid;
    _playGame(arkanoid);
}

void marduino_play()
{
    //_playGame(marduino_setup, marduino_loop);
}

void maze_play()
{
    Maze maze;
     _playGame(maze);
}

void towers_play()
{
    Towers towers;
     _playGame(towers);
}


/*
void super_crate_play()
{
     _playGame(super_crate_buino_setup, super_crate_buino_loop);
}*/

