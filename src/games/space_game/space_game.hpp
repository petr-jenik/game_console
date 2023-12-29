#pragma once

//#include <queue> // std::queue
//#include <deque> // std::deque
#include "gameAPI.h"
#include "player.hpp"

#include "stars.hpp"

class SpaceGame : public GameAPI
{
private:
    Startfield mStarfield;
    Player mPlayer;
    Deque<Enemy> mEnemies;
    Deque<Projectile> mMyProjectiles;
    Deque<Projectile> mEnemyProjectiles;
    uint32_t mCycleCounter;
public:
    SpaceGame();
    void setup();
    void loop();
};