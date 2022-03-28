#include "gui.h"
#include "util.h"

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
    setlocale(LC_ALL, "");

    initscr();
    cbreak(); noecho(); keypad(win, TRUE);
    clear();
    curs_set(0);
    start_color();

    Map map;

    genRandom(map); //Generate the rooms (do not have walls yet)
    genWalls(map);  //Add walls around the rooms
    genColors(map, none_toggle); //Add colors and text to all tile types

    addDoor(map); //Add doors to rooms and add hallways between doors of different rooms
    none_toggle = true;
    genColors(map, none_toggle); //Update the new tiles with colors again
    px = map.rooms[0].lowerx-2; //Set user to coordinates of room[0]
    py = map.rooms[0].lowery-2;

    while(1){

        turns++;

        for (int j = 0; j < LINES; j++){
            for (int i = 0; i < COLS / 2; i++){

                int x = i + camx - COLS / 4; 
                int y = j + camy - LINES / 2;

                if (x < 0 || y < 0 || x >= sizex || y >= sizey) continue;

                //Uncover tiles
                if (map.discovered[y][x] < 1){
                    map.discovered[y][x] = calcLighting(map, px, py, x, y, 8);
                }

                //Refresh Display Tile
                float v = map.discovered[y][x];

                if (x == px && y == py){ //Display Player
                    printT(i*2,j, "Al" ,255,255,1,  200,200,1);
                }else{ //Display Tile
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
        if (ch == 68){ //Left
            px--;
        }
        if (ch == 67){ //Right
            px++;
        }
        if (ch == 65){ //Up
            py--;
        }
        if (ch == 66){ //Down
            py++;
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

    endwin();
}

