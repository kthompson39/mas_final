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

int Agent::BFS_to_Undiscovered(int y, int x){
    m_map.extra[y][x] = 1;
    std::vector<Tree> leaves;

    Tree l;
    l.L = y;
    l.R = x;
    l.parent = -1;
    leaves.push_back(l);

    for(int n = 0; n < 1000; n++){
        int a = 0;
        int mm = -1;
        for (auto elem : leaves){
            mm++;

            auto j = elem.L;
            auto i = elem.R;

            if (j == m_goalY && i == m_goalX){ //If BFS has found agent's goal coordinates
                int point = mm;
                while(1){
                    int yy = leaves[point].L;
                    int xx = leaves[point].R;
                    point = leaves[point].parent;
                    if (point == 0){
                        m_x = xx;
                        m_y = yy;
                    }
                    if (point == -1) break;
                }
                a = 1;
                break;
            }

            if ( j < 1 || i < 1 || j > sizey-1 || i > sizex-1) continue;
            if ( isTileOccupiable(i+1,j,m_map) && m_map.extra[j][i+1] == 0 ){
                Tree p;p.L = j;p.R = i+1;p.parent = mm;leaves.push_back(p);m_map.extra[p.L][p.R] = 1;
            }
            if ( isTileOccupiable(i-1,j,m_map) && m_map.extra[j][i-1] == 0 ){
                Tree p;p.L = j;p.R = i-1;p.parent = mm;leaves.push_back(p);m_map.extra[p.L][p.R] = 1;
            }
            if ( isTileOccupiable(i,j+1,m_map) && m_map.extra[j+1][i] == 0 ){
                Tree p;p.L = j+1;p.R = i;p.parent = mm;leaves.push_back(p);m_map.extra[p.L][p.R] = 1;
            }
            if ( isTileOccupiable(i,j-1,m_map) && m_map.extra[j-1][i] == 0){
                Tree p;p.L = j-1;p.R = i;p.parent = mm;leaves.push_back(p);m_map.extra[p.L][p.R] = 1;
            }
            //Diagonal tiles
            if ( isTileOccupiable(i+1,j+1,m_map) && m_map.extra[j+1][i+1] == 0 ){
                Tree p;p.L = j+1;p.R = i+1;p.parent = mm;leaves.push_back(p);m_map.extra[p.L][p.R] = 1;
            }
            if ( isTileOccupiable(i-1,j-1,m_map) && m_map.extra[j-1][i-1] == 0 ){
                Tree p;p.L = j-1;p.R = i-1;p.parent = mm;leaves.push_back(p);m_map.extra[p.L][p.R] = 1;
            }
            if ( isTileOccupiable(i-1,j+1,m_map) && m_map.extra[j+1][i-1] == 0 ){
                Tree p;p.L = j+1;p.R = i-1;p.parent = mm;leaves.push_back(p);m_map.extra[p.L][p.R] = 1;
            }
            if ( isTileOccupiable(i+1,j-1,m_map)  && m_map.extra[j-1][i+1] == 0){
                Tree p;p.L = j-1;p.R = i+1;p.parent = mm;leaves.push_back(p);m_map.extra[p.L][p.R] = 1;
            }

        }
        if (a) break;
    }       
    for (int j = 0; j < sizey; j++){
        for (int i = 0; i < sizex; i++){
            m_map.extra[j][i] = 0;
        }
    } 
    return 0;
}


void Agent::step(std::vector<Agent>& agents, Map& map)
{
    updateInternalMap(map);


    if (m_goalX == m_x && m_goalY == m_y){
        m_aimless = true; //Agent no longer has a goal
    }

    //If agent has no goal yet...set a goal to an undiscovered floor tile
    if ((m_goalX == -1 && m_goalY == -1) || m_aimless == true){
        //Look through all tiles in map that match certain criteria
        int check = 0;
        for (int x = 0; x < sizex; x++){
            for (int y = 0; y < sizey; y++){
                if (m_global_map.tiles[y][x] == Floor && m_global_map.discovered[y][x] < .5){
                    check++;
                }
            }
        }
        //Randomly select a tile to make the new goal
        if (check > 0){
            int r = rand()%check; check = 0;
            for (int x = 0; x < sizex; x++){
                for (int y = 0; y < sizey; y++){
                    if (m_global_map.tiles[y][x] == Floor && m_global_map.discovered[y][x] < .5){
                        check++;
                        if (r == check){
                            m_goalX = x;
                            m_goalY = y;
                            m_aimless = false; //Agent is no longer aimless (has a goal)
                        }
                    }
                }
            }
        }
    }

    BFS_to_Undiscovered(m_y, m_x);

    //Only perform if agent has become aimless
    if (m_aimless == true){
        auto possible_directions = getMovementDirections(m_x, m_y, map);
        int action = rand() % possible_directions.size();
        //select random direction to walk
        moveDirection(possible_directions[action]);
    }
}

