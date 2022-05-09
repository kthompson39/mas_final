# MAS Final: Treasure Hunt

## Dependencies:
This program relies on C++14, Make, and the NCurses library. Make sure all are installed before running.
Additionally, one should use a terminal with full 256-bit color support to view the full experience.

## Running the Program

Once the code has been cloned, cd to the root of the directory and enter:

```
$ make
```

This will build the source files and create a binary called `a.out`. To run this, type:

```
$ ./a.out
```

Once the program is started, the screen will go blank. This is normal. To make the program 
run, hold down any key (except a special key) on the keyboard. Every time a key is entered, the program will progress
one time tick. Thus, holding down a key will continually input character and progress the program
quickly. 

## Special Keys

While running the program, if one presses the 'up' key, the whole map is revealed. This can be nice to view the whole
map in full color.

Another special key is the 'down' key. This restarts the same map, but does not keep the same agent behavior. 

## Program Output

Once the program finishes, details and statistics of the run will be output. This includes:
* The final health values of all agents
* The treasure counts of the agents
* The likableness table of all agents
* The history of teams that an agent was a part of
* Total time ticks of the program
* The winner
* Total likableness of the winner for all other agents
* The seed for the run. This can be put back in the program (in `mas.cpp`)to recreate this run

## Adjusting Settings

All settings that can be modified are in `definitions.h`, except for changing the seed. That is done at the
top of main in `mas.cpp`.



