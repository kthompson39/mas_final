#include "util.h"
#include "definitions.h"
#include <algorithm>

//Generate random rooms and also push new room struct to "rooms" vector
void genRandom(Map& map){
    for (int y = 0; y < sizey; y++){
        for (int x = 0; x < sizex; x++){
            map.tiles[y][x] = None;
            map.blocks[y][x] = 0;
            map.word[y][x] = "  ";
            map.discovered[y][x] = 0.2;

            if (y <= 7){ //Top left corner becomes grass
                if (x <  5) map.tiles[y][x] = Grass;
                if (x == 5) map.tiles[y][x] = None;
                if (x <= 5 && y == 7) map.tiles[y][x] = None;
            }

            int a = rand()%1; 
            int w = rand()%7+5;
            int h = rand()%7+5;
            //w = 6; h =6; //Uncomment for very uniform rooms
            if (a == 0 && y > h+2 && x > w+5 && x < sizex-2 && y < sizey-2){ //Do not create room too close to edge of area
                bool c = true;

                //Check the new room is not overlapping another room
                for (int y2 = 0; y2 < h+1; y2++){
                    for (int x2 = 0; x2 < w+1; x2++){
                        if(map.tiles[y - y2][x - x2] == Floor || 
                           map.tiles[y - y2][x - x2 + 2] == Floor){
                            c = false;
                        }
                    }
                }

                //Create new room
                if (c){
                    for (int y2 = 0; y2 < h; y2++){
                        for (int x2 = 0; x2 < w; x2++){
                            if(map.tiles[y - y2][x - x2] == None){
                                map.tiles[y - y2][x - x2] = Floor;
                            }
                        }
                    }
                    Room r; //Add info to the room struct to be able to find room later
                    r.lowerx = x; //Sets coordinates of bottom right corner of room
                    r.lowery = y;
                    r.room_w = w;
                    r.room_h = h;
                    map.rooms.push_back(r);
                }
            }
        }
    }
}

//Essentially BFS algorithm to reach a tile equal to "find" and change all tiles along the shortest path between the points to "change"
//y and x are the starting coordinates
void Dij(Map& map, int y, int x, enum category find, enum category change){
    map.extra[y][x] = 1;
    std::vector<Tree> leaves;

    Tree l;
    l.L = y;
    l.R = x;
    l.parent = -1;
    leaves.push_back(l);

    for(int n = 0; n < 50; n++){
        int a = 0;
        int mm = -1;
        for (auto elem : leaves){
            mm++;

            auto j = elem.L;
            auto i = elem.R;

            if (map.tiles[j][i] == find ) {
                int point = mm;
                point = leaves[point].parent; //Do not change the final destination tile
                while(1){
                    int yy = leaves[point].L;
                    int xx = leaves[point].R;
                    map.tiles[yy][xx] = change;
                    map.word[yy][xx] = "  ";
                    point = leaves[point].parent;
                    if (point == -1) break;
                }
                a = 1;
                break;
            }

            if ( j < 1 || i < 1 || j > sizey-1 || i > sizex-1) continue;
            if ( map.blocks[j][i+1] == 0 && map.extra[j][i+1] == 0 ){
                Tree p;
                p.L = j;
                p.R = i+1;
                p.parent = mm;
                leaves.push_back(p);
                map.extra[p.L][p.R] = 1;
            }
            if ( map.blocks[j][i-1] == 0 && map.extra[j][i-1] == 0 ){
                Tree p;
                p.L = j;
                p.R = i-1;
                p.parent = mm;
                leaves.push_back(p);
                map.extra[p.L][p.R] = 1;
            }
            if ( map.blocks[j+1][i] == 0 && map.extra[j+1][i] == 0 ){
                Tree p;
                p.L = j+1;
                p.R = i;
                p.parent = mm;
                leaves.push_back(p);
                map.extra[p.L][p.R] = 1;
            }
            if ( map.blocks[j-1][i] == 0  && map.extra[j-1][i] == 0){
                Tree p;
                p.L = j-1;
                p.R = i;
                p.parent = mm;
                leaves.push_back(p);
                map.extra[p.L][p.R] = 1;
            }
            //Diagonal tiles
            if ( map.blocks[j+1][i+1] == 0 && map.extra[j+1][i+1] == 0 ){
                Tree p;
                p.L = j+1;
                p.R = i+1;
                p.parent = mm;
                leaves.push_back(p);
                map.extra[p.L][p.R] = 1;
            }
            if ( map.blocks[j-1][i-1] == 0 && map.extra[j-1][i-1] == 0 ){
                Tree p;
                p.L = j-1;
                p.R = i-1;
                p.parent = mm;
                leaves.push_back(p);
                map.extra[p.L][p.R] = 1;
            }
            if ( map.blocks[j+1][i-1] == 0 && map.extra[j+1][i-1] == 0 ){
                Tree p;
                p.L = j+1;
                p.R = i-1;
                p.parent = mm;
                leaves.push_back(p);
                map.extra[p.L][p.R] = 1;
            }
            if ( map.blocks[j-1][i+1] == 0  && map.extra[j-1][i+1] == 0){
                Tree p;
                p.L = j-1;
                p.R = i+1;
                p.parent = mm;
                leaves.push_back(p);
                map.extra[p.L][p.R] = 1;
            }

        }
        if (a) break;
    }       
    for (int j = 0; j < sizey; j++){
        for (int i = 0; i < sizex; i++){
            map.extra[j][i] = 0;
        }
    } 
}


void removeDirection(std::vector<Direction>& vec, Direction d)
{
    vec.erase(std::remove(vec.begin(), vec.end(), d), vec.end());
}

bool isTileOccupiable(int x, int y, Map& map)
{
    if(x < 0 || x > sizex)
    {
        return false;
    }
    if(y < 0 || y > sizey)
    {
        return false;
    }

    if(map.tiles[y][x] == Grass ||
       map.tiles[y][x] == Floor || 
       map.tiles[y][x] == Hallway || 
       map.tiles[y][x] == Door)
    {
        return true;
    }
    else
    {
        return false;
    }
}

vector<Direction> getMovementDirections(int x, int y, Map& map)
{
    std::vector<Direction> possible_directions{
        STAY
    };

    if(isTileOccupiable(x-1,y,map))
        possible_directions.push_back(LEFT);
    if(isTileOccupiable(x+1,y,map))
        possible_directions.push_back(RIGHT);
    if(isTileOccupiable(x,y-1,map))
        possible_directions.push_back(UP);
    if(isTileOccupiable(x,y+1,map))
        possible_directions.push_back(DOWN);
    if(isTileOccupiable(x-1,y-1,map))
        possible_directions.push_back(UP_LEFT);
    if(isTileOccupiable(x+1,y-1,map))
        possible_directions.push_back(UP_RIGHT);
    if(isTileOccupiable(x-1,y+1,map))
        possible_directions.push_back(DOWN_LEFT);
    if(isTileOccupiable(x+1,y+1,map)) 
        possible_directions.push_back(DOWN_RIGHT);

    return possible_directions;
}

std::vector<Agent> createAgents(int numAgents, int startX, int startY, Map& map)
{
    std::vector<Agent> agents;
    for(int i = 0; i < numAgents; i++)
    {
        agents.push_back(Agent(startX,startY, numAgents, map));
    }

    return agents;
}

int isAgentOnTile(int x, int y, std::vector<Agent>& agents, Map& map)
{
    for(size_t i = 0; i < agents.size(); i++)
    {
        if(agents[i].m_x == x && agents[i].m_y == y)
        {
            return i;
        }
    }
    return -1;
}

