#include <algorithm>
#include "agent.h"
#include "gui.h"
#include "util.h"
#include "definitions.h"

Agent::Agent(int startX, int startY, int numOtherAgents, Map& map, int id) :
    m_x(startX), m_y(startY), m_likableness(std::vector<int>(numOtherAgents, INITIAL_LIKABLENESS_VALUE)), 
    m_map(Map(map)), m_global_map(map), m_id(id)
{
}

void Agent::joinAuction()
{
    m_collectStep = -1;
    m_inAuction = true;
}

void Agent::leaveAuction()
{
    m_collectStep = -1;
    m_inAuction = false;
    m_aimless = true;
}

bool Agent::bid(int askingPrice, std::vector<int> agentsInAuction)
{
    //TODO implement agent bidding in auctions
    return rand()%2;
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

            m_map.tiles[y][x] = m_global_map.tiles[y][x];
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


// int xi = 0;
// int yi = 0;

int Agent::BFS_to_Undiscovered(int y, int x, bool option){
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

            if (j == m_goalY && i == m_goalX && option == false){ //If BFS has found agent's goal coordinates
                int point = mm;
                while(1){
                    int yy = leaves[point].L;
                    int xx = leaves[point].R;
                    point = leaves[point].parent;
                    if (point == 0){ //Option = false if handling movement
                        m_x = xx;
                        m_y = yy;
                    }
                    if (point == -1) break;
                }
                a = 1;
                break;
            }
            if (m_global_map.tiles[j][i] != TreasureTile
                && (j == m_y && i == m_x)==false && option == true){ 
                //Place a new treasure after agent perishes
                int yi = leaves[mm].L;
                int xi = leaves[mm].R;

                m_global_map.tiles[yi][xi] = TreasureTile;
                m_global_map.treasures.push_back(Treasure{xi,yi,1});
                updateTileColor(m_global_map, true, yi, xi);

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

    if(m_health <= 0)
    {
        while(m_treasureCount > 0){
            BFS_to_Undiscovered(m_y, m_x, true);

            /*for (Agent& agent: agents){
                agent.m_map.tiles[yi][xi] = TreasureTile;
                agent.m_map.treasures.push_back(Treasure{xi,yi,1});
                updateTileColor(agent.m_map, true, yi, xi);
            }*/

            m_treasureCount--;
        }
    }

    if(m_stuck || m_inAuction || m_health <= 0)
    {
        return;
    }

    // if agent is collecting treasure, continue collecting treasure
    if(m_collectStep > -1)
    {
        m_collectStep++;
        return;
    }


    if (m_goalX == m_x && m_goalY == m_y){
        // if agent is on treasure, start collecting it
        if(m_map.tiles[m_y][m_x] == TreasureTile)
        {
            m_collectStep = 0;
        }
        else
        {
            m_aimless = true; //Agent no longer has a goal
        }
    }

    int steal_treasure = 1;

    //If agent has no goal yet...set a goal to an undiscovered floor tile
    if ((m_goalX == -1 && m_goalY == -1) || m_aimless == true){
        //Look through all tiles in map that match certain criteria
        int check_t = 0; //Count treasure tiles
        int check_d = 0; //Count tiles to discover
        int check_m = 0; //Count agents with treasures in order to mug

        for (int x = 0; x < sizex; x++){
            for (int y = 0; y < sizey; y++){

                //Various conditions to search for
                if (m_map.tiles[y][x] == TreasureTile && m_map.discovered[y][x] > .8) check_t++;
                if (isTileOccupiable(x, y, m_map) && m_map.discovered[y][x] < .8) check_d++;
            
                for (Agent& agent: agents){
                    if(agent.m_health > 0 && agent.m_treasureCount > steal_treasure 
                        && m_id != agent.m_id && m_targetId == -1) check_m++;
                }

            }
        }

        //Agents will explore by default. But will seek Treasure if any treasure lies in their discovered m_map array.
        std::string      agent_top_want = "Explore";
        if (check_m > 0) agent_top_want = "Mug";
        if (check_t > 0) agent_top_want = "Treasure";

        if (check_t > 0 || check_d > 0 || check_m > 0){
            int r_t = 0; int r_d = 0; int r_m = 0;
            
            if (check_t > 0){
                r_t = rand()%check_t; check_t = 0;
            }
            if (check_d > 0){
                r_d = rand()%check_d; check_d = 0;
            }
            if (check_m > 0){
                r_m = rand()%check_m; check_m = 0;
            }

            for (int x = 0; x < sizex; x++){
                for (int y = 0; y < sizey; y++){

                    //Check for same conditions as above
                    if (m_map.tiles[y][x] == TreasureTile && m_map.discovered[y][x] > .8 && agent_top_want == "Treasure"){
                        if (r_t == check_t){
                            m_goalX = x;
                            m_goalY = y;
                            m_aimless = false; //Agent is no longer aimless (has a goal to go to Treasure coordinates)
                        }
                        check_t++;
                    }
                    if (agent_top_want == "Explore" && isTileOccupiable(x, y, m_map) && m_map.discovered[y][x] < .8){

                        if (r_d == check_d){
                            m_goalX = x;
                            m_goalY = y;
                            m_aimless = false; //Agent is no longer aimless (has a goal to go to Undiscovered Tile within its own map array)
                        }
                        check_d++;
                    }
                    if (agent_top_want == "Mug"){
                        for (Agent& agent: agents){
                            if(agent.m_health > 0 && agent.m_treasureCount > steal_treasure 
                                && m_id != agent.m_id && m_targetId == -1){

                                if (r_m == check_m){
                                    m_targetId = agent.m_id;
                                }
                                check_m++;
                            }
                        }
                    }

                }
            }
        }else{
            m_goalX = 1; //If no objectives remain, head to top left corner
            m_goalY = 1;
        }
    }


    ///////////MUGGING
    if (m_targetId != -1){ //Current target coordinate is another agent
        for (Agent& agent: agents)
        {
            if (agent.m_id == m_targetId){
                m_goalX = agent.m_x; //Set goal coordinates to target agent coordinates
                m_goalY = agent.m_y;
                if ((m_x == m_goalX && m_y == m_goalY) 
                || (m_x+1 == m_goalX && m_y == m_goalY)
                || (m_x-1 == m_goalX && m_y == m_goalY)
                || (m_x == m_goalX && m_y+1 == m_goalY)
                || (m_x == m_goalX && m_y-1 == m_goalY)){ //If arrive at agent, mug target

                    m_treasureCount += 1;
                    agent.m_treasureCount -= 1;
                    agent.m_health -= 10;
                    m_targetId = -1;
                }
            }
        }
    }


    BFS_to_Undiscovered(m_y, m_x, false);

    //Only perform if agent has become aimless
    if (m_aimless == true){
        auto possible_directions = getMovementDirections(m_x, m_y, map);
        int action = rand() % possible_directions.size();
        //select random direction to walk
        moveDirection(possible_directions[action]);
    }
}

