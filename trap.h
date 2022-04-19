#pragma once
#include <vector>

struct Trap {
    int x;
    int y;
    bool isSprung;
    std::vector<int> agentsTrapped;
};
