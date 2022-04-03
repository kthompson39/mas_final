#pragma once

enum Direction {LEFT, RIGHT, UP, DOWN, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT, STAY};

//Used only in Dij(...)
struct Tree {
    int L;
    int R;
    int index;
    int parent;
};

enum category{None, Grass, Wall, Floor, Hallway, Door, River};

