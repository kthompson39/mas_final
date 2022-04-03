#pragma once

#include <vector>
#include "map.h"
#include "definitions.h"

class Agent {
public:
    Agent(int startX, int startY, int numOtherAgents, Map& map);

    int updateInternalMap(Map& fullMap);
    void step(std::vector<Agent>& agents, Map& fullMap);
    void moveDirection(Direction d);
    
    int m_x;
    int m_y;
    int m_treasureCount;
    std::vector<int> m_likableness;

    int m_goalX;
    int m_goalY;

    Map m_map;
    Map& m_global_map;
};
