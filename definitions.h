#pragma once

enum Direction {LEFT, RIGHT, UP, DOWN, UP_RIGHT, UP_LEFT, DOWN_RIGHT, DOWN_LEFT, STAY};

static const int MAX_TRAPS_PER_ROOM = 6;
static const int MAX_TREASURES_PER_ROOM = 6;
static const int TREASURE_COLLECTION_TIME = 10;
static const int INITIAL_LIKABLENESS_VALUE = 50;
static const int INITIAL_AGENT_HEALTH_VALUE = 80;
static const int TRAP_DAMAGE_PER_TICK = 1;
static const int MUG_THRESHOLD = 5;
static const int NOTICE_AGENTS_DISTANCE = 40; //used only for mugging, not saving (there is infinite distance to save)
static const int TEAM_CLOSENESS = 4; //radius by which agents in team realize they are too far and should get closer to each other
static const int SAME_TEAM_BONUS = 5; //If very high, teams will never mug each other

//Used only in Dij(...)
struct Tree {
    int L;
    int R;
    int index;
    int parent;
};

enum category{None, Grass, Wall, Floor, Hallway, Door, River, TrapTile, TreasureTile};

