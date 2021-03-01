# Assignment 2

This program simulates conway's game of life in C programming
The Game of Life should be played on a potentially infinite, two-dimensional (2-D) grid of cells that represents a universe. 
Each cell has two possible states: dead or alive. The game progresses through generations. 
There are only three rules that determine the state of the universe after each generation: 
1. Any live cell with two or three live neighbors survives. 
2. Any dead cell with exactly three live neighbors becomes a live cell. 
3. All other cells die, either due to loneliness or overcrowding.


## Build

        $ clang -Wall -Wextra -Werror -Wpedantic -o life life.o universe.o -lncurses
        
## Running

        $ ./life
        
## Cleaning
        
        rm -f life *.o
        
## format

        clang-format -i -style=file life.c universe.c
