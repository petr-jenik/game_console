#pragma once

#include <cstdint>

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
* @brief Position of any item on the game area.
*/
typedef struct 
{
    int32_t x;
    int32_t y;
    Direction eDirection;
} Position;