#include <algorithm>
#include "agent.h"
#include "gui.h"
#include "util.h"
#include "definitions.h"

Agent::Agent(int startX, int startY, int numOtherAgents, Map& map, int id) :
    m_x(startX), m_y(startY), m_likableness(std::vector<long int>(numOtherAgents, INITIAL_LIKABLENESS_VALUE)), 
    m_map(Map(map)), m_global_map(map), m_id(id), m_teamHistory(std::vector<int>())
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
    return rand()%2;
}

bool Agent::isWillingToChangeToAgentsTeams(Agent& agent, std::map<int,float> allMeanTeamLikableness, std::map<int,int> allHighestTeamLikablenessValues)
{
    if(agent.m_team == NO_TEAM || agent.m_id == m_id)
        return false;

    if(m_likableness[agent.m_id] > JOIN_TEAM_THRESHOLD
       && m_likableness[agent.m_id] > allHighestTeamLikablenessValues[m_team] 
       && allMeanTeamLikableness[agent.m_team] > allMeanTeamLikableness[m_team]
      )
    {
        return true;
    }

    return false;
}

int Agent::updateInternalMap(Map& fullMap)
{
    float brightness = 8;
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
            m_map.onTop[y][x] = m_global_map.onTop[y][x];
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
            if (m_global_map.onTop[j][i] != TreasureTile
                && (j == m_y && i == m_x)==false && option == true){ 
                //Place a new treasure after agent perishes
                int yi = leaves[mm].L;
                int xi = leaves[mm].R;

                m_global_map.onTop[yi][xi] = TreasureTile;
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

void Agent::joinAgentsTeam(Agent& agent)
{
    // create new team
    if(agent.m_team == NO_TEAM)
    {
        agent.m_team = rand()%1000;
        agent.m_teamHistory.push_back(agent.m_team);
    }

    m_team = agent.m_team;
    m_teamHistory.push_back(m_team);
}

void Agent::step(std::vector<Agent>& agents, Map& map)
{
    updateInternalMap(map);

    if(m_team == -1) //agent currently has no team
    {
        m_team = (m_id % 2)+5; //initialize with 2 teams. +5 so color begins at yellow
        m_team = m_id;
        // m_team = 5; 
        m_teamHistory.push_back(m_team);
    }

    if(m_health <= 0)
    {
        while(m_treasureCount > 0){
            BFS_to_Undiscovered(m_y, m_x, true);
            m_treasureCount--;
        }
        if (m_deathBy == -1){
            if (m_stuck == true){
                m_stuck = false;
                m_deathBy = 1;
            }else{
                m_deathBy = 0;
            }
        }
    }

    if(m_stuck || m_inAuction || m_health <= 0 || m_gone)
    {
        return;
    }

    // if agent is collecting treasure, continue collecting treasure
    if(m_collectStep > -1 && m_map.onTop[m_y][m_x] == TreasureTile)
    {
        m_collectStep++;
        return;
    }
    //else
    //{
    //    m_collectStep = -1;
    //}

    if ((m_x <= 0 || m_y <= 0)){
        m_gone = true;
        return;
    }

    if (m_goalX == m_x && m_goalY == m_y){
        // if agent is on treasure, start collecting it
        if(m_map.onTop[m_y][m_x] == TreasureTile)
        {
            m_collectStep = 0;
            //return;
        }
        else
        {
            m_aimless = true; //Agent no longer has a goal
            // m_desireToMug = false; //Agent no longer attempting to mug someone
        }
    }

    //int steal_treasure = 4; //agents mug after this many treasures

    for (Agent& agent: agents){ //If target agent is dead, find new goal
        if (agent.m_id == m_targetId && agent.m_health <= 0){
            m_aimless = true;
            m_desireToMug = false;
            m_targetId = -1;
        }

        if (agent.m_id == m_targetId && agent.m_health > 0){
            m_goalX = agent.m_x;
            m_goalY = agent.m_y;
        }

        //ensure teams are close
        if (agent.m_team == m_team && m_team != NO_TEAM && agent.m_health > 0 
            && agent.m_id != m_id && agent.m_stuck == false
            && agent.m_id < m_id && agent.m_gone == false
            && agent.m_gone == false){ 
            float a = agent.m_x - m_x;
            float b = agent.m_y - m_y;
            float c = sqrt(a*a + b*b);
            if (c > TEAM_CLOSENESS){ 
                m_goalX = agent.m_goalX;
                m_goalY = agent.m_goalY;
                m_aimless = false;
                m_desireToMug = agent.m_desireToMug; 
                m_targetId = agent.m_targetId; //Agents in group all will want to mug same agent
            }
        }
    }


    //If agent has no goal yet...set a goal to an undiscovered floor tile
    if ((m_goalX == -1 && m_goalY == -1) || m_aimless == true){

        //////////////////////////////////////
        //calculate team statistics
        //////////////////////////////////////
        
        // dictionary of <team, mean team likeablness for this agent>
        std::map<int,float> allMeanTeamLikableness;
        // dictionary of <team, highest team likableness value for this agent>
        std::map<int,int> allHighestTeamLikablenessValues;
        // dictionary of <team, team sizes>
        std::map<int,int> allTeamSizes;

        for(Agent& agent: agents)
        {
            if(agent.m_id != m_id && agent.m_team != NO_TEAM)
            {
                // initialize dicstionary if not
                if(allHighestTeamLikablenessValues.count(agent.m_team))
                {
                    allHighestTeamLikablenessValues[agent.m_team] = 0;
                }
                // initialize dicstionary if not
                if(allMeanTeamLikableness.count(agent.m_team))
                {
                    allMeanTeamLikableness[agent.m_team] = 0;
                }
                // initialize dicstionary if not
                if(allTeamSizes.count(agent.m_team))
                {
                    allTeamSizes[agent.m_team] = 0;
                }

                if(m_likableness[agent.m_id] > allHighestTeamLikablenessValues[agent.m_team])
                    allHighestTeamLikablenessValues[agent.m_team] = m_likableness[agent.m_id];

                allMeanTeamLikableness[agent.m_team] += m_likableness[agent.m_id];
                allTeamSizes[agent.m_team] += 1;
            }
        }

        // calculate mean values
        for(auto it : allTeamSizes)
        {
            if(it.second != 0)
                allMeanTeamLikableness[it.first] /= it.second;
        }

        //Look through all tiles in map that match certain criteria
        int check_t = 0; //Count treasure tiles
        int check_d = 0; //Count tiles to discover
        int check_m = 0; //Count agents with treasures in order to mug
        int check_s = 0; //Count agents in traps to save
        int check_j = 0; //Count agents to join a team with

        for (int x = 0; x < sizex; x++){
            for (int y = 0; y < sizey; y++){

                //Various conditions to search for
                if (m_map.onTop[y][x] == TreasureTile && m_map.discovered[y][x] > .8) check_t++;
                if (isTileOccupiable(x, y, m_map) && m_map.discovered[y][x] < .8) check_d++;
            }
        }
            
        for (Agent& agent: agents){
            float a = agent.m_x - m_x;
            float b = agent.m_y - m_y;
            float c = sqrt(a*a + b*b);
            int same_team = 1; //Agents less likely to steal from own team
            if (agent.m_team == m_team && m_team != NO_TEAM) same_team = SAME_TEAM_BONUS; 

            //CHECK IF AGENT WORTH MUGGING
            if(agent.m_health > 0 
                    && m_id != agent.m_id 
                    && m_targetId == -1
                    && c <= NOTICE_AGENTS_DISTANCE
                    //&& (
                    //    (
                           //If prefer not to check for 2 agents treasure difference...
                           //&& agent.m_treasureCount > (MUG_THRESHOLD*same_team) 
                    //     agent.m_treasureCount-m_treasureCount > (steal_treasure*same_team) 
                    && m_likableness[agent.m_id] <= LIKABLENESS_MUG_THRESHOLD
                    //    )
                    //    || m_likableness[agent.m_id] <= LIKABLENESS_MUG_THRESHOLD/2
                    //   )
              )
                check_m++;

            //CHECK IF AGENT WORTH SAVING
            if(agent.m_health > 0 
                    && agent.m_stuck == true 
                    && m_id != agent.m_id 
                    && m_targetId == -1
                    && m_likableness[agent.m_id] >= LIKABLENESS_SAVE_THRESHOLD)
                check_s++;

            if(agent.m_health > 0 
                    && m_id != agent.m_id 
                    && m_targetId == -1
                    && c <= NOTICE_AGENTS_DISTANCE
                    && (m_team != agent.m_team || m_team == NO_TEAM)
                    && isWillingToChangeToAgentsTeams(agent, allMeanTeamLikableness, allHighestTeamLikablenessValues))
                check_j++;
        }



        //Agents will explore by default. But will seek Treasure if any treasure lies in their discovered m_map array.
        std::string      agent_top_want = "Explore";
        if (check_j > 0) agent_top_want = "Join";
        if (check_t > 0) agent_top_want = "Treasure";
        if (check_m > 0) agent_top_want = "Mug";
        if (check_s > 0) agent_top_want = "Save";

        if (check_t > 0 || check_d > 0 || check_m > 0 || check_s > 0 ){//|| check_j > 0){
            int r_t = 0; int r_d = 0; int r_m = 0; int r_s = 0; int r_j = 0;

            if (check_t > 0){
                r_t = rand()%check_t; check_t = 0;
            }
            if (check_d > 0){
                r_d = rand()%check_d; check_d = 0;
            }
            if (check_m > 0){
                r_m = rand()%check_m; check_m = 0;
            }
            if (check_s > 0){
                r_s = rand()%check_s; check_s = 0;
            }
            if (check_j > 0){
                r_j = rand()%check_j; check_j = 0;
            }

            for (int x = 0; x < sizex; x++){
                for (int y = 0; y < sizey; y++){

                    //Check for same conditions as above
                    if (agent_top_want == "Treasure" && m_map.onTop[y][x] == TreasureTile && m_map.discovered[y][x] > .8){
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
                }
            }
            for (Agent& agent: agents){

                float a = agent.m_x - m_x;
                float b = agent.m_y - m_y;
                float c = sqrt(a*a + b*b);
                int same_team = 1;
                if (agent.m_team == m_team && m_team != NO_TEAM) same_team = 3;

                if(agent_top_want == "Mug" && agent.m_health > 0 
                        && m_id != agent.m_id && m_targetId == -1
                        && c <= NOTICE_AGENTS_DISTANCE
                        //&& (
                        //    (
                               //If prefer not to check for 2 agents treasure difference...
                               //&& agent.m_treasureCount > (MUG_THRESHOLD*same_team) 
                        //     agent.m_treasureCount-m_treasureCount > (steal_treasure*same_team) 
                        && m_likableness[agent.m_id] <= LIKABLENESS_MUG_THRESHOLD
                        //    )
                        //    || m_likableness[agent.m_id] <= LIKABLENESS_MUG_THRESHOLD/2
                        //   )
                  )
                {

                    if (r_m == check_m){
                        m_targetId = agent.m_id;
                        m_desireToMug = true;
                    }
                    check_m++;
                }
                if(agent_top_want == "Save" && agent.m_health > 0 
                        && agent.m_stuck == true 
                        && m_id != agent.m_id && m_targetId == -1
                        && m_likableness[agent.m_id] >= LIKABLENESS_SAVE_THRESHOLD)
                {

                    if (r_s == check_s){
                        m_targetId = agent.m_id;
                    }
                    check_s++;
                }
                if(agent_top_want == "Join" && agent.m_health > 0 
                        && m_id != agent.m_id 
                        && m_targetId == -1
                        && c <= NOTICE_AGENTS_DISTANCE
                        && (m_team != agent.m_team || m_team == NO_TEAM)
                        && isWillingToChangeToAgentsTeams(agent, allMeanTeamLikableness, allHighestTeamLikablenessValues))
                {
                    if (r_j == check_j)
                    {
                        joinAgentsTeam(agent);
                    }
                    check_j++;

                }
            }
        }else{
            m_goalX = 0; //If no objectives remain, head to top left corner
            m_goalY = 0;
        }
    }


    if (m_targetId != -1){ //Current target coordinate is another agent
        for (Agent& agent: agents)
        {
            ///////////MUGGING
            if (agent.m_id == m_targetId && m_desireToMug == true){
                m_goalX = agent.m_x; //Set goal coordinates to target agent coordinates
                m_goalY = agent.m_y;
                if ((m_x == m_goalX && m_y == m_goalY) 
                || (m_x+1 == m_goalX && m_y == m_goalY)
                || (m_x-1 == m_goalX && m_y == m_goalY)
                || (m_x == m_goalX && m_y+1 == m_goalY)
                || (m_x == m_goalX && m_y-1 == m_goalY)
                || (m_x+1 == m_goalX && m_y+1 == m_goalY) 
                || (m_x+1 == m_goalX && m_y-1 == m_goalY) 
                || (m_x-1 == m_goalX && m_y+1 == m_goalY) 
                || (m_x-1 == m_goalX && m_y-1 == m_goalY) ){

                    if (agent.m_treasureCount > 0){ // && m_likableness[m_targetId] < 0){
                        agent.m_likableness[m_id] -= MUGGING_LIKABLENESS_REDUCTION;
                        // if agent is on a team, then all of their team members will dislike current agent
                        if(agent.m_team != NO_TEAM)
                        {
                            for(Agent& teammate : agents)
                            {
                                if(agent.m_team == teammate.m_team && m_id != teammate.m_id)
                                {
                                    teammate.m_likableness[m_id] -= MUGGING_LIKABLENESS_REDUCTION / 2;

                                }
                            }
                        }
                        if (m_team == agent.m_team) 
                        {
                            m_team = NO_TEAM; //rand()%1000; //Attacking agent takes new team if both on same team
                            m_teamHistory.push_back(m_team);
                        }

                        m_treasureCount += 1;
                        m_targetId = -1;
                        m_aimless = true;
                        m_desireToMug = false;
                        m_likableness[agent.m_id] -= 1;

                        agent.m_treasureCount -= 1;
                        agent.m_health -= 10;
                        agent.m_hurt = 10; //Font will appear red for 10 time steps
                        agent.m_aimless = true;
                        // agent.m_goalX = agent.m_x; 
                        // agent.m_goalY = agent.m_y;
                        // agent.m_targetId = m_id;
                        // agent.m_desireToMug = true;
                    }
                }
            }
            ///////////SAVE FROM TRAP
            if (agent.m_id == m_targetId){
                m_goalX = agent.m_x; //Set goal coordinates to target agent coordinates
                m_goalY = agent.m_y;
                if ((m_x == m_goalX && m_y == m_goalY) 
                || (m_x+1 == m_goalX && m_y == m_goalY)
                || (m_x-1 == m_goalX && m_y == m_goalY)
                || (m_x == m_goalX && m_y+1 == m_goalY)
                || (m_x == m_goalX && m_y-1 == m_goalY)
                || (m_x+1 == m_goalX && m_y+1 == m_goalY) 
                || (m_x+1 == m_goalX && m_y-1 == m_goalY) 
                || (m_x-1 == m_goalX && m_y+1 == m_goalY) 
                || (m_x-1 == m_goalX && m_y-1 == m_goalY) ){

                    //m_global_map.tiles[agent.m_y][agent.m_x] = Floor;
                    m_global_map.onTop[agent.m_y][agent.m_x] = None;
                    updateTileColor(m_global_map, false, agent.m_y,agent.m_x);

                    agent.m_stuck = false;
                    agent.m_x = m_x;
                    agent.m_y = m_y;
                    agent.joinAgentsTeam(*this); //Bring over to new team if saved
                    // the saved person likes this agent more
                    agent.m_likableness[m_id] += SAVING_LIKABLENESS_ADDITION;
                    for(Agent& teammate : agents)
                    {
                        if(teammate.m_team == agent.m_team)
                        {
                            if(m_id != teammate.m_id)
                            {
                                // teamates slightly like the person who saves other agents
                                teammate.m_likableness[m_id] += SAVING_LIKABLENESS_ADDITION/4;
                            }
                            if(agent.m_id != teammate.m_id)
                            {
                                // the agent who was just saved slightly likes their new teamates
                                agent.m_likableness[teammate.m_id] += SAVING_LIKABLENESS_ADDITION/4;
                            }
                        }
                    }

                    m_targetId = -1;
                    m_aimless = true;
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

