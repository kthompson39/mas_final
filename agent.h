#pragma once

#include <vector>
#include <map>
#include "map.h"
#include "definitions.h"

class Agent {
public:
    Agent(int startX, int startY, int numOtherAgents, Map& map, int id);

    int updateInternalMap(Map& fullMap);
    void step(std::vector<Agent>& agents, Map& fullMap);
    void moveDirection(Direction d);
    int BFS_to_Undiscovered(int, int, bool);
    void joinAuction();
    void leaveAuction();
    bool bid(int askingPrice, std::vector<int> agentsInAuction);
    void joinAgentsTeam(Agent& agent);
    bool isWillingToChangeToAgentsTeams(Agent& agent, std::map<int,float> allMeanTeamLikableness, std::map<int,int> allHighestTeamLikablenessValues);
    
    int m_x = -1;
    int m_y = -1;
    int m_treasureCount = 0;
    std::vector<long int> m_likableness;

    int m_goalX = -1;
    int m_goalY = -1;

    Map m_map;
    Map& m_global_map;

    bool m_gone = false;
    int m_hurt = 0;
    int m_team = -1;
    bool m_aimless = false;
    int m_collectStep = -1;
    bool m_stuck = false;
    bool m_inAuction = false;
    int m_health = INITIAL_AGENT_HEALTH_VALUE;
    int m_id = -1;
    int m_targetId = -1;
    bool m_desireToMug = false;
    int m_deathBy = -1; //-1 if not dead. 0 is trap, 1 is mug

    std::vector<int> m_teamHistory;
};
