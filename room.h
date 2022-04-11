#pragma once

#include <vector>
#include "treasure.h"
#include "trap.h"

struct Room {
    int lowerx; //Coordinates of lower right corner 
    int lowery;
    int room_w; //Room width
    int room_h; //Room height
    int doorx; //Door coordinates
    int doory;
};

