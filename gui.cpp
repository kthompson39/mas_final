#include "gui.h"
#include "util.h"

using namespace std;

void printT(int x, int y, string s, int F_R, int F_G, int F_B,   int B_R, int B_G, int B_B){
    gotoxy(x+1,y+1);
    printf("\033[48;2;%d;%d;%dm", B_R, B_G, B_B);
    printf("\033[38;2;%d;%d;%dm", F_R, F_G, F_B);
    printf("%s\033[0m", s.c_str());
}

float surroundingTilesFour(Map& map, int y, int x, int against){
    float a = 0;
    if (map.tiles[y][x+1] == against) a+=1;
    if (map.tiles[y][x-1] == against) a+=1;
    if (map.tiles[y+1][x] == against) a+=1;
    if (map.tiles[y-1][x] == against) a+=1;
    return a;
}

float surroundingTilesAdj(Map& map, int y, int x, int against){
    float a = 0;
    if (map.tiles[y][x+1] == against) a+=1;
    if (map.tiles[y][x-1] == against) a+=1;
    if (map.tiles[y+1][x] == against) a+=1;
    if (map.tiles[y-1][x] == against) a+=1;
    if (map.tiles[y+1][x+1] == against) a+=.25;
    if (map.tiles[y-1][x+1] == against) a+=.25;
    if (map.tiles[y+1][x-1] == against) a+=.25;
    if (map.tiles[y-1][x-1] == against) a+=.25;
    return a;
}

void rgb(Map& map, int y, int x, int a, int b, int c, int d, int e, int f){
    map.f_r[y][x] = a;
    map.f_g[y][x] = b;
    map.f_b[y][x] = c;
    map.b_r[y][x] = d;
    map.b_g[y][x] = e;
    map.b_b[y][x] = f;
}


string chooseWall(Map& map, enum category c, int y, int x, int rando){
    string a = "╬ "; //4-way intersect, default

    if (rando == 1){
        int e = rand()%16;
        if (e == 0) a = "╬ ";
        if (e == 1) a = "║ ";
        if (e == 2) a = "══";
        if (e == 3) a = "╬═";
        if (e == 4) a = "╣ ";
        if (e == 5) a = "╠═";
        if (e == 6) a = "╩═";
        if (e == 7) a = "╦═";
        if (e == 8) a = "╝ ";
        if (e == 9) a = "╚═";
        if (e == 10) a = "╗ ";
        if (e == 11) a = "╔═";
        return a;
    }
    //Check each side 1 by 1
    if (map.tiles[y][x+1] != c && map.tiles[y][x-1] != c && map.tiles[y+1][x] != c && map.tiles[y-1][x] != c) a = "╬ "; //single pillar █
    if (map.tiles[y][x+1] != c && map.tiles[y][x-1] != c && map.tiles[y+1][x] == c && map.tiles[y-1][x] == c) a = "║ "; //vertical
    if (map.tiles[y][x+1] == c && map.tiles[y][x-1] == c && map.tiles[y+1][x] != c && map.tiles[y-1][x] != c) a = "══"; //horizontal
    if (map.tiles[y][x+1] == c && map.tiles[y][x-1] == c && map.tiles[y+1][x] == c && map.tiles[y-1][x] == c) a = "╬═"; //4-way

    if (map.tiles[y][x+1] != c && map.tiles[y][x-1] == c && map.tiles[y+1][x] == c && map.tiles[y-1][x] == c) a = "╣ "; //3-way, no right
    if (map.tiles[y][x+1] == c && map.tiles[y][x-1] != c && map.tiles[y+1][x] == c && map.tiles[y-1][x] == c) a = "╠═"; //3-way, no left
    if (map.tiles[y][x+1] == c && map.tiles[y][x-1] == c && map.tiles[y+1][x] != c && map.tiles[y-1][x] == c) a = "╩═"; //3-way, no top
    if (map.tiles[y][x+1] == c && map.tiles[y][x-1] == c && map.tiles[y+1][x] == c && map.tiles[y-1][x] != c) a = "╦═"; //3-way, no bottom

    if (map.tiles[y][x+1] != c && map.tiles[y][x-1] == c && map.tiles[y+1][x] != c && map.tiles[y-1][x] == c) a = "╝ "; //2-way, no right bottom
    if (map.tiles[y][x+1] == c && map.tiles[y][x-1] != c && map.tiles[y+1][x] != c && map.tiles[y-1][x] == c) a = "╚═"; //2-way, no left bottom
    if (map.tiles[y][x+1] != c && map.tiles[y][x-1] == c && map.tiles[y+1][x] == c && map.tiles[y-1][x] != c) a = "╗ "; //2-way, no right top
    if (map.tiles[y][x+1] == c && map.tiles[y][x-1] != c && map.tiles[y+1][x] == c && map.tiles[y-1][x] != c) a = "╔═"; //2-way, no left top

    if (map.tiles[y][x+1] == c && map.tiles[y][x-1] != c && map.tiles[y+1][x] != c && map.tiles[y-1][x] != c) a = "══";
    if (map.tiles[y][x+1] != c && map.tiles[y][x-1] == c && map.tiles[y+1][x] != c && map.tiles[y-1][x] != c) a = "══";
    if (map.tiles[y][x+1] != c && map.tiles[y][x-1] != c && map.tiles[y+1][x] != c && map.tiles[y-1][x] == c) a = "║ "; //2

    return a;
}

void updateTileColor(Map& map, bool none_toggle, int y, int x)
{
    int f = 15;
    if (map.tiles[y][x] == None){
        map.blocks[y][x] = none_toggle; //Blocks is TRUE after generating Hallways
        map.word[y][x] = "  ";
        rgb(map, y,x, 0,0,0, 0,0,0);
        if (surroundingTilesAdj(map, y,x,Hallway) || surroundingTilesAdj(map, y,x,River)  || surroundingTilesAdj(map, y,x,Grass)){
            map.word[y][x] = "##";
            rgb(map, y,x, 122,103,58, 0,0,0);
        }
    }
    if (map.tiles[y][x] == Grass){ 
        int r = rand() % 15;
        map.blocks[y][x] = 0;
        if (r == 0) map.word[y][x] = " '";
        if (r == 1) map.word[y][x] = " ,";
        if (r == 2) map.word[y][x] = "' ";
        if (r == 3) map.word[y][x] = ", "; 
        rgb(map, y,x, 100-f, 255-rand() % 100-f, 100-f,   1, rand() % 25+75-f, 1);

    }else if (map.tiles[y][x] == Floor){ 
        map.word[y][x] = "  "; 
        map.blocks[y][x] = 0;
        int s = rand()%15;
        rgb(map, y,x, 200,200,200,  100-s,100-s,100-s);
    }
    else if (map.tiles[y][x] == Hallway){ 
        map.word[y][x] = "##"; 
        map.blocks[y][x] = 0;
        int s = rand()%15;
        rgb(map, y,x, 150,150,150,  60-s,60-s,60-s);
    }
    else if (map.tiles[y][x] == Wall){ 
        map.word[y][x] = chooseWall(map, Wall, y,x,0);
        map.blocks[y][x] = 1; //Blocks is TRUE
        int s = rand()%15;
        rgb(map, y,x, 200,200,200,  160-s,160-s,160-s);
    }
    else if (map.tiles[y][x] == Door){ 
        map.word[y][x] = "[]";
        map.blocks[y][x] = 0;
        int s = rand()%15;
        if (!surroundingTilesAdj(map, y,x,Grass)){
            rgb(map, y,x, 100,200,200,  60-s,110-s,110-s);
        }else{
            rgb(map, y,x, 200,200,200,   1, rand() % 25+75-f, 1);
        }
    } else if (map.tiles[y][x] == River){ 
        int r = rand() % 8;
        map.blocks[y][x] = 0;
        map.word[y][x] = "  ";
        if (r == 0) map.word[y][x] = "≈≈";
        if (r == 1) map.word[y][x] = "≈ ";
        if (r == 2) map.word[y][x] = " ≈";
        if (r == 3) map.word[y][x] = " ~";
        if (r == 4) map.word[y][x] = "~ "; 
        if (r == 5) map.word[y][x] = "~~"; 
        rgb(map, y,x, 50,50, 255-rand() % 10,   0,0,rand() % 25+150-f);

    }
    else if (map.tiles[y][x] == TrapTile){ 
        map.word[y][x] = "^^"; 
        map.blocks[y][x] = 0;
        int s = rand()%15;
        rgb(map, y,x, 255,50,50,  200-s,50-s,50-s);
    }
    else if (map.tiles[y][x] == TreasureTile){ 
        map.word[y][x] = "$$"; 
        map.blocks[y][x] = 0;
        rgb(map, y,x, 255,180,1,  200,130,1);
    }
}

//Give tiles certain colors and properties such as blocking or not
void genColors(Map& map, bool none_toggle){

    for (int y = 0; y < sizey; y++) {
        for (int x = 0; x < sizex; x++) {
            updateTileColor(map, none_toggle, y, x);
        }
    }
}

//Just used for make walls appear near other tile types (i.e. Floor and Hallway)
void genWalls(Map& map){
    for (int y = 0; y < sizey; y++) {
        for (int x = 0; x < sizex; x++) {
            if (map.tiles[y][x] == Floor){ //Add walls on floors that are nearby None
                if (surroundingTilesAdj(map,y,x,None)){
                    map.tiles[y][x] = Wall;
                }
            }
        }
    }
}

void genRiver(Map& map){
    int skip = 0;
    int shift = rand()%15-7;
    if (shift == 0) shift = 1;
    for (int y = 0; y < sizey; y++) {
        for (int x = 0; x < sizex; x++) {
            int ys = (y/shift)-(sizey/2/shift)-3;
            if (x <= sizex/2-ys+3 && x >= sizex/2-ys){

                if (map.tiles[y][x] == None)
                    map.tiles[y][x] = River; 
                int s = rand()%16;
                skip++;

            }
        }
    }
}

float calcLighting(Map& map, int posx, int posy, int x, int y, float brightness){

    float taper = 1.0 / brightness;

    //If tile too far, auto no light
    float a = posx - x;
    float b = posy - y;
    float c = 1-sqrt(a*a+b*b) * taper;
    if (c <= 0){ 
        return 0;
    }

    //Ray Tracing
    int blocking = 0;
    for (float d = 0.1; d < brightness; d+= c*.9 ){
        float ry = posy-y; float rx = posx-x;
        ry *= (d*taper); rx *= (d*taper); 
        int xl = posx-int(rx);
        int yl = posy-int(ry);
        if ( xl < 0 || yl < 0 || xl >= sizex || yl >= sizey ){ 
            blocking = 1; break; }
        if ( map.blocks[yl][xl] == 1 ){
            blocking = 1;
            break;
        }
    }

    //Final decision
    if (blocking == 0){
        c = sqrt(c);
        if (c >= 1) c*=2;
        return 1;
        // return c; //Return c instead for distance based brightness
    }
    return 0;
}

//Adds doors to rooms by recalling the room information from the "rooms" vector
void addDoor(Map& map){
    for (int i = 0; i < map.rooms.size(); i++){
        int dir = rand()%4;
        int sx, sy; //start x and start y
        if (dir == 0){ //Right side
            sy = map.rooms[i].lowery - map.rooms[i].room_h/2;
            sx = map.rooms[i].lowerx;
            map.tiles[sy][sx+1] = Hallway;
        }
        if (dir == 1){ //Left side
            sx = map.rooms[i].lowerx - map.rooms[i].room_w+1;
            sy = map.rooms[i].lowery - map.rooms[i].room_h/2;
            map.tiles[sy][sx-1] = Hallway;
        }
        if (dir == 2){ //Top side
            sy = map.rooms[i].lowery - map.rooms[i].room_h+1;
            sx = map.rooms[i].lowerx - map.rooms[i].room_w/2;
            map.tiles[sy-1][sx] = Hallway;
        }
        if (dir == 3){ //Bottom side
            sy = map.rooms[i].lowery;
            sx = map.rooms[i].lowerx - map.rooms[i].room_w/2;
            map.tiles[sy+1][sx] = Hallway;
        }
        
        Dij(map, sy,sx, Door, Hallway); //y,x start, find Road, Change to Hallway along shortest path
        map.tiles[sy][sx] = Door;
        map.blocks[sy][sx] = 0;
        map.rooms[i].doorx = sx;
        map.rooms[i].doory = sy;
        
    }

    //Make room of index 0 be different
    // for (int x = 0; x < map.rooms[0].room_w; x++){
        // for (int y = 0; y < map.rooms[0].room_h; y++){
            // if (map.tiles[map.rooms[0].lowery - y][map.rooms[0].lowerx - x] != Door)
                // map.tiles[map.rooms[0].lowery - y][map.rooms[0].lowerx - x] = Grass;
        // }
    // }

    int a = rand()%4+1; //Adds door near grass
    Dij(map, 7,a,Door,Hallway);
    // map.tiles[7][a] = Door;

}
