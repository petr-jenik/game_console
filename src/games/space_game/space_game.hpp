#pragma once

#include <queue> // std::queue

#include "gameAPI.h"
#include "player.hpp"
#include "enemy.hpp"
#include "projectile.hpp"
#include "stars.hpp"

class SpaceGame : public GameAPI
{
private:
    Startfield mStarfield;
    Player mPlayer;


    // Deque was used to be able to access individual elements by the corresponding index.
    std::deque<Enemy> mEnemies;
    std::deque<Projectile> mMyProjectiles;
    std::deque<Projectile> mEnemyProjectiles;
    uint32_t mCycleCounter;
public:
    SpaceGame();
    void setup();
    void loop();
};