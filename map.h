#pragma once

#include <string>
#include <vector>
#include <list>

#include "room.h"
#include "treasure.h"
#include "trap.h"
#include "definitions.h"

using namespace std;

const int sizex = 45; //Size of map
const int sizey = 45;

class Map {
  public:
    Map();
    Map(const Map&);
    void updateOnTop();

    enum category tiles[sizey][sizex];
    enum category onTop[sizey][sizex];
    string word[sizey][sizex];
    int f_r[sizey][sizex]; //foreground_red
    int f_g[sizey][sizex]; //foreground_green
    int f_b[sizey][sizex]; //foreground_blue
    int b_r[sizey][sizex]; //background_red
    int b_g[sizey][sizex]; //background_green
    int b_b[sizey][sizex]; //background_blue
    int extra[sizey][sizex]; //Additional info if needed (used in BFS in function Dij...)
    float discovered[sizey][sizex]; //Light value of the tile. 0 to 1 range
    int blocks[sizey][sizex]; //Whether the tile blocks or not (0 if not, 1 if yes)
    std::list<Treasure> treasures;
    std::list<Trap> traps;
    vector<Room> rooms; //Room vector, contains info for every room created
};

