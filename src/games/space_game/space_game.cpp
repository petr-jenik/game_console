#include "space_game.hpp"
#include "my_gui.h"
#include "stars.hpp"

SpaceGame::SpaceGame()
:mCycleCounter(0)
{
    // Empty constructor
}

void SpaceGame::setup()
{
    // Show welcome screen
    GUI& gui = gui.getInstance();
    gui.welcomeScreen("Space game - setup");
}

void SpaceGame::loop()
{
    // Show welcome screen
    GUI& gui = gui.getInstance();
    gui.display.clearDisplay();
    //gui.welcomeScreen("Space game - loop");

    /*if ((mCycleCounter / 15) % 2 == 0)
    {
        this->mStarfield.move(true);
    }
    else
    {
        this->mStarfield.move(false);
    }*/

    mStarfield.update();
    mStarfield.draw(gui);


    // Update cycle counter
    mCycleCounter++;
}
