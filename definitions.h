#pragma once

enum Direction {LEFT, RIGHT, UP, DOWN, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT, STAY};

static const int MAX_TRAPS_PER_ROOM = 3;
static const int MAX_TREASURES_PER_ROOM = 5;
static const int TREASURE_COLLECTION_TIME = 10;
static const int INITIAL_LIKABLENESS_VALUE = 50;

//Used only in Dij(...)
struct Tree {
    int L;
    int R;
    int index;
    int parent;
};

enum category{None, Grass, Wall, Floor, Hallway, Door, River, TrapTile, TreasureTile};

