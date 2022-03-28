#pragma once

#include "map.h"

//Generate random rooms and also push new room struct to "rooms" vector
void genRandom(Map& map);

//Used only in Dij(...)
struct Tree {
    int L;
    int R;
    int index;
    int parent;
};

//Essentially BFS algorithm to reach a tile equal to "find" and change all tiles along the shortest path between the points to "change"
//y and x are the starting coordinates
void Dij(Map& map, int y, int x, enum category find, enum category change);

