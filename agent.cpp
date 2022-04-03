#include <algorithm>
#include "agent.h"
#include "gui.h"
#include "util.h"

Agent::Agent(int startX, int startY, int numOtherAgents, Map& map) :
    m_x(startX), m_y(startY), m_likableness(std::vector<int>(numOtherAgents, 50)), m_map(Map(map)), m_global_map(map)
{
}


int Agent::updateInternalMap(Map& fullMap)
{
    float brightness = 8; // ????
    int min_x = std::max(m_x - 7, 0);
    int min_y = std::max(m_y - 7, 0);

    int max_x = std::min(m_x + 7, sizex);
    int max_y = std::min(m_y + 7, sizey);

    int discovered = 0;

    for(int y = min_y; y < max_y; y++)
    {
        for(int x = min_x; x < max_x; x++)
        {
            if (m_map.discovered[y][x] < 1){
                float new_l = calcLighting(m_map, m_x, m_y, x, y, brightness);
                if (new_l > m_map.discovered[y][x])
                {
                    m_map.discovered[y][x] = new_l;
                    m_global_map.discovered[y][x] = std::max(new_l, m_global_map.discovered[y][x]);
                    discovered += 1;
                }
            }
        }
    }
    return discovered;
}

void Agent::moveDirection(Direction d)
{
    if(d == LEFT) 
        m_x -= 1;
    if(d == RIGHT) 
        m_x += 1;
    if(d == UP) 
        m_y -= 1;
    if(d == DOWN) 
        m_y += 1;
    if(d == UP_LEFT) 
    {
        m_y -= 1;
        m_x -= 1;
    }
    if(d == UP_RIGHT)
    {
        m_y -= 1;
        m_x += 1;
    }
    if(d == DOWN_LEFT)
    {
        m_y += 1;
        m_x -= 1;
    }
    if(d == DOWN_RIGHT)
    {
        m_y += 1;
        m_x += 1;
    }
}

void Agent::step(std::vector<Agent>& agents, Map& map)
{
    updateInternalMap(map);

    auto possible_directions = getMovementDirections(m_x, m_y, map);

    int action = rand() % possible_directions.size();

    // select random direction to walk
    moveDirection(possible_directions[action]);
}

