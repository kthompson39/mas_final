#pragma once

#include "map.h"
#include "agent.h"
#include "definitions.h"
//#include "treasure.h"
//#include "trap.h"
#include "location.h"
#include <vector>
#include <list>

//Generate random rooms and also push new room struct to "rooms" vector
void genRandom(Map& map);
std::list<Location> genTreasure(Room& room);
std::list<Location> genTrap(Room& room, std::list<Location> invalid_locations);

//Essentially BFS algorithm to reach a tile equal to "find" and change all tiles along the shortest path between the points to "change"
//y and x are the starting coordinates
void Dij(Map& map, int y, int x, enum category find, enum category change);

vector<Direction> getMovementDirections(int x, int y, Map& map);

std::vector<Agent> createAgents(int numAgents, int startX, int startY, Map& map);

int isAgentOnTile(int x, int y, std::vector<Agent>& agents, Map& map);
std::vector<int> getAgentsOnTile(int x, int y, std::vector<Agent>& agents, Map& map);

bool isTileOccupiable(int x, int y, Map& map);

/*
 * This assumes that 
 */
template <typename Container>
void removeSubset(Container& fullList, std::vector<int> subset)
{
    int i = 0;
    int j = 0;
    for (auto itr = fullList.cbegin(); itr != fullList.end(); itr++)
    {
        // remove items that are in the subset
        if (i == subset[j]) {
            fullList.erase(itr--);
            j++;
        }
        i++;
    }
}
