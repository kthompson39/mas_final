#include <vector>
#include <string>
#include "gui.h"
#include "util.h"
#include "auction.h"
#include "agent.h"

using namespace std;

int px = 5; //User x and y coordinates
int py = 5;
int camx = px; //Camera coordinates
int camy = py;

int ch; //Global char input
int turns = 0;
bool none_toggle = false;


int main(int argc, char *argv[]){

    srand(time(0));

    WINDOW* win = stdscr;
    bool show_all_map = false;
    setlocale(LC_ALL, "");

    initscr();
    cbreak(); noecho(); keypad(win, TRUE);
    clear();
    curs_set(0);
    start_color();

    //Map map;
    Map original_map;


    genRandom(original_map); //Generate the rooms (do not have walls yet)
    genWalls(original_map);  //Add walls around the rooms
    genColors(original_map, none_toggle); //Add colors and text to all tile types

    addDoor(original_map); //Add doors to rooms and add hallways between doors of different rooms
    genRiver(original_map);
    genWalls(original_map);

    none_toggle = true;
    genColors(original_map, none_toggle); //Update the new tiles with colors again
    //px = map.rooms[0].lowerx-2; //Set user to coordinates of room[0]
    //py = map.rooms[0].lowery-2;

    while(1)
    {
        Map map(original_map);
        genColors(map, none_toggle);
        auto agents = createAgents(6, 1,1, map);

        // holds current, ongoing auctions
        std::list<Auction> auctions;

        turns = 0;

        while(1){

            turns++;

            for (Agent& agent: agents)
            {
                agent.step(agents, map);
            }


            /////////////////////////////////////////////////////////
            // TRAPS
            /////////////////////////////////////////////////////////
            std::vector<int> trap_subset_indices;
            int k = 0;
            for(Trap& trap: map.traps)
            {
                if(trap.isSprung)
                {
                    std::vector<int> dead_agents_indices;
                    int d = 0;
                    for(int agentId: trap.agentsTrapped)
                    {
                        //Bandaid solution to dying after being saved...
                        if(agents[agentId].m_stuck == true) 
                            agents[agentId].m_health -= TRAP_DAMAGE_PER_TICK;
                        if(agents[agentId].m_health <= 0)
                        {
                            dead_agents_indices.push_back(d);
                        }
                        d++;
                    }
                    if(dead_agents_indices.size() > 0)
                    {
                        removeSubset(trap.agentsTrapped, dead_agents_indices);
                    }
                }
                else // trap is not sprung
                {
                    std::vector<int> agentsOnTile = getAgentsOnTile(trap.x, trap.y, agents, map);
                    if(agentsOnTile.size() > 0)
                    {
                        for(int agentId: agentsOnTile)
                        {
                            agents[agentId].m_stuck = true;
                            trap.agentsTrapped.push_back(agentId);
                        }
                        trap.isSprung = true;
                    }

                }

                if(trap.isSprung && trap.agentsTrapped.size() == 0)
                {
                    trap_subset_indices.push_back(k);

                    // update map
                    map.onTop[trap.y][trap.x] = None;
                    // map.word[trap.y][trap.x] = "  ";
                    updateTileColor(map, none_toggle, trap.y, trap.x);
                }
                k++;
            }
            // remove sprung traps 
            if(trap_subset_indices.size() > 0)
            {
                removeSubset(map.traps, trap_subset_indices);
            }


            /////////////////////////////////////////////////////////
            // TREASURES
            /////////////////////////////////////////////////////////
            std::vector<int> treasure_subset_indices;
            k = 0;
            for(Treasure& treasure: map.treasures)
            {
                std::vector<int> agentsOnTile = getAgentsOnTile(treasure.x, treasure.y, agents, map);

                if(agentsOnTile.size() == 1)
                {
                    int i = agentsOnTile[0];
                    // if agent collected treasure for full time
                    if(agents[i].m_collectStep >= TREASURE_COLLECTION_TIME)
                    {
                        // remove treasure
                        treasure_subset_indices.push_back(k);

                        // give agent treasure
                        agents[i].m_collectStep = -1;
                        agents[i].m_treasureCount += treasure.value;

                        // update map
                        map.onTop[treasure.y][treasure.x] = None;
                        // map.word[treasure.y][treasure.x] = "  ";
                        updateTileColor(map, none_toggle, treasure.y, treasure.x);
                    }
                }
                // start auction for treasure
                else if(agentsOnTile.size() > 1)
                {
                    // start auction with agents
                    auctions.push_back(Auction(agents, agentsOnTile, treasure));
                    for(int i: agentsOnTile)
                    {
                        agents[i].joinAuction();
                    }

                    // remove treasure from treasure list since its now in an auction
                    treasure_subset_indices.push_back(k);
                    // update map
                    map.onTop[treasure.y][treasure.x] = None;
                    // map.word[treasure.y][treasure.x] = "  ";
                    updateTileColor(map, none_toggle, treasure.y, treasure.x);
                }

                k++;
            }
            // remove collected treasure
            if(treasure_subset_indices.size() > 0)
            {                
                removeSubset(map.treasures, treasure_subset_indices);
            }
            map.updateOnTop();


            /////////////////////////////////////////////////////////
            // AUCTIONS
            /////////////////////////////////////////////////////////
            std::vector<int> auction_subset_indices;
            k = 0;
            for(Auction& auction: auctions)
            {
                auction.step();
                if(auction.m_isOver)
                {
                    auction_subset_indices.push_back(k);
                }
                k++;
            }
            // remove completed auctions 
            if(auction_subset_indices.size() > 0)
            {
                removeSubset(auctions, auction_subset_indices);
            }

            /////////////////////////////////////////////////////////////////////////

            for (int j = 0; j < LINES; j++){
                for (int i = 0; i < COLS / 2; i++){

                    int x = i + camx - COLS / 4; 
                    int y = j + camy - LINES / 2;

                    if (x < 0 || y < 0 || x >= sizex || y >= sizey) continue;

                    //Uncover tiles
                    if (map.discovered[y][x] < 1){
                        float new_l = calcLighting(map, px, py, x, y, 8);
                        if (new_l > map.discovered[y][x])
                            map.discovered[y][x] = new_l;
                    }

                    //Refresh Display Tile
                    float v = 0.3;
                    for (Agent& agent: agents)
                    {
                        if (agent.m_health > 0){
                            v = agent.m_map.discovered[y][x]; //Show map of last indexed agent
                            px = agent.m_x; 
                            py = agent.m_y; //Causes camera to follow this agent
                            if (v < .8) v = .4;
                        }
                    }
                    if (show_all_map)
                        v = 1; //Uncomment for complete brightness 

                    //v = map.discovered[y][x]; //Show global map
                    int agentOnTile = isAgentOnTile(x, y, agents, map);


                    if (x == px && y == py){ //Display Player
                        // printT(i*2,j, "Al" ,255,255,1,  200,200,1);
                    }

                    if(agentOnTile > -1)
                    {
                        std::string agent_name = std::to_string(agentOnTile) + " ";

                        if (agents[agentOnTile].m_health > 0){
                            if (agents[agentOnTile].m_hurt <= 0)
                            {
                                printT(i*2,j, agent_name ,0,0,0,  200*v,200*v,1);
                            }
                            else
                            {
                                agents[agentOnTile].m_hurt--;
                                printT(i*2,j, agent_name ,255*v,0,0,  200*v,200*v,1);
                            }
                        }else{
                            printT(i*2,j, agent_name ,255*v,0,0,  150*v,1,1);
                        }

                    }
                    else{ //Display Tile

                        printT(i*2,j, 
                            map.word[y][x] ,
                            map.f_r[y][x]*v, 
                            map.f_g[y][x]*v, 
                            map.f_b[y][x]*v, 
                            map.b_r[y][x]*v, 
                            map.b_g[y][x]*v, 
                            map.b_b[y][x]*v);
                    }
                }
            }

            ch = getch();
            int oldx = px;
            int oldy = py;
            //oldx = agents[0].m_x; 
            //oldy = agents[0].m_y;

            if (ch == 68){ //Left
                px--;
            }
            if (ch == 67){ //Right
                px++;
            }
            if (ch == 65){ //Up
                py--;
                show_all_map = 1 - show_all_map;
            }
            if (ch == 66){ //Down
                py++;
                break;
            }
            if (ch == KEY_F(1))
                break;

            if (px < 0 || px >= sizex || py < 0 || py >= sizey){
                px = oldx; py = oldy;
            }

            //Update camera position if moved too far 
            oldx = camx;
            oldy = camy;

            if (px-camx > COLS / 8){
                camx = px + COLS / 8;
            }
            if (px-camx < -COLS / 8){
                camx = px - COLS / 8;
            }
            if (py-camy > LINES / 4){
                camy = py + LINES / 4;
            }
            if (py-camy < -LINES / 4){
                camy = py - LINES / 4;
            }
            if (camx != oldx || camy != oldy){
                camx = px; camy = py;
            }

            if (camx < COLS/4) camx = COLS/4;
            if (camy < LINES/2) camy = LINES/2;
            if (camx > sizex-COLS/4) camx = sizex-COLS/4;
            if (camy > sizey-LINES/2) camy = sizey-LINES/2;

        }
    }

    endwin();
}

