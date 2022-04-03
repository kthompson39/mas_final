#pragma once

#include "map.h"
#include "agent.h"
#include "definitions.h"
#include <vector>

//Generate random rooms and also push new room struct to "rooms" vector
void genRandom(Map& map);

//Essentially BFS algorithm to reach a tile equal to "find" and change all tiles along the shortest path between the points to "change"
//y and x are the starting coordinates
void Dij(Map& map, int y, int x, enum category find, enum category change);

vector<Direction> getMovementDirections(int x, int y, Map& map);

std::vector<Agent> createAgents(int numAgents, int startX, int startY, Map& map);

int isAgentOnTile(int x, int y, std::vector<Agent>& agents, Map& map);

