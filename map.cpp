#include "map.h"

template <class T>
void copyVector(const T (&fromVec)[sizey][sizex], T (&toVec)[sizey][sizex])
{
    for(int i = 0; i < sizey; i++)
    {
        for(int j = 0; j < sizex; j++)
        {
            toVec[i][j] = fromVec[i][j];
        }
    }
}

Map::Map()
{
}

Map::Map(const Map& map)
{
    copyVector(map.discovered, this->discovered);
    copyVector(map.tiles, this->tiles);
    copyVector(map.blocks, this->blocks);

    copyVector(map.word, this->word);
    copyVector(map.f_r, this->f_r);
    copyVector(map.f_g, this->f_g);
    copyVector(map.f_b, this->f_b);

    copyVector(map.b_r, this->b_r);
    copyVector(map.b_g, this->b_g);
    copyVector(map.b_b, this->b_b);

    copyVector(map.extra, this->extra);

    this->rooms = std::vector<Room>(map.rooms);
    this->treasures = std::list<Treasure>(map.treasures);
    this->traps = std::list<Trap>(map.traps);
}

