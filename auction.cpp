#include "auction.h"


Auction::Auction(std::vector<Agent>& agents, std::vector<int> agentsInAuction, Treasure& treasure) :
    m_agents(agents), m_agentsInAuction(agentsInAuction), m_treasure(treasure)
    //m_inAuction(std::vector<bool>(std::vector<bool>(agents.size(), true)))
{
}

void Auction::giveWinTo(int agent_index)
{
    // give treasure to winner
    m_agents[agent_index].m_treasureCount += m_treasure.value;

    // change likeableness values for all agent involved in auction
    for(int i: m_agentsInAuction)
    {
        if(i != agent_index)
        {
            m_agents[i].m_likableness[agent_index] -= m_askingPrice;
        }

        m_agents[i].leaveAuction();
    }
}

void Auction::step()
{
    m_askingPrice += 5;
    giveWinTo(m_agentsInAuction[0]);

    m_isOver = true;

    return;



    //TODO finish implementing auction step
    m_askingPrice += 5;

    std::vector<int> agentsStayingIn;

    for(int i: m_agentsInAuction)
    {
        bool isIn = m_agents[i].bid(m_askingPrice, m_agentsInAuction);
        if(isIn)
        {
            agentsStayingIn.push_back(i);
        }
    }
    if(agentsStayingIn.size() == 1)
    {
        // winner
    }
    else if(agentsStayingIn.size() == 0)
    {
        // tie breaker
    }


}
