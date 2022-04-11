#pragma once

#include <vector>
#include "agent.h"
#include "treasure.h"

class Auction
{
  public:
      Auction(std::vector<Agent>& agents, std::vector<int> agentsInAuction, Treasure& treasure);
      void step();
      void giveWinTo(int);

      std::vector<Agent>& m_agents;
      std::vector<int> m_agentsInAuction;
      //std::vector<bool> m_inAuction;
      Treasure m_treasure;
      //int m_step = -1;
      int m_askingPrice = 0;
      bool m_isOver = false;
};
