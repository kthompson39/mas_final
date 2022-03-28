#pragma once

#define _XOPEN_SOURCE_EXTENDED
#define _GNU_SOURCE

#include <ncurses.h>
#include <spawn.h>
#include <math.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <wchar.h>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <time.h>
#include <cstdio>

#include "room.h"
#include "map.h"
using namespace std;

#define gotoxy(x,y) printf("\033[%d;%dH",(y),(x))

void printT(int x, int y, string s, int F_R, int F_G, int F_B,   int B_R, int B_G, int B_B);


float surroundingTilesFour(Map& map, int y, int x, int against);

float surroundingTilesAdj(Map& map, int y, int x, int against);

void rgb(Map& map, int y, int x, int a, int b, int c, int d, int e, int f);

string chooseWall(Map& map, enum category c, int y, int x, int rando);

//Give tiles certain colors and properties such as blocking or not
void genColors(Map& map, bool);

//Just used for make walls appear
void genWalls(Map& map);

float calcLighting(Map& m, int posx, int posy, int x, int y, float brightness);

void addDoor(Map& map);

