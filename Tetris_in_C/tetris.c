#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "tetris.h"

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

tetris_location TETROMINOS[NUM_TETROMINOS][NUM_ORIENTATIONS][TETRIS] = {
    // I
    {{{1, 0}, {1, 1}, {1, 2}, {1, 3}},
     {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
     {{3, 0}, {3, 1}, {3, 2}, {3, 3}},
     {{0, 1}, {1, 1}, {2, 1}, {3, 1}}},
    // J
    {{{0, 0}, {1, 0}, {1, 1}, {1, 2}},
     {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
     {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
     {{0, 1}, {1, 1}, {2, 0}, {2, 1}}},
    // L
    {{{0, 2}, {1, 0}, {1, 1}, {1, 2}},
     {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
     {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
     {{0, 0}, {0, 1}, {1, 1}, {2, 1}}},
    // O
    {{{0, 1}, {0, 2}, {1, 1}, {1, 2}},
     {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
     {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
     {{0, 1}, {0, 2}, {1, 1}, {1, 2}}},
    // S
    {{{0, 1}, {0, 2}, {1, 0}, {1, 1}},
     {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
     {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
     {{0, 0}, {1, 0}, {1, 1}, {2, 1}}},
    // T
    {{{0, 1}, {1, 0}, {1, 1}, {1, 2}},
     {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
     {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
     {{0, 1}, {1, 0}, {1, 1}, {2, 1}}},
    // Z
    {{{0, 0}, {0, 1}, {1, 1}, {1, 2}},
     {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
     {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
     {{0, 1}, {1, 0}, {1, 1}, {2, 0}}},
};

int GRAVITY_LEVEL[MAX_LEVEL+1] = {
// 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
  50, 48, 46, 44, 42, 40, 38, 36, 34, 32,
//10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  30, 28, 26, 24, 22, 20, 16, 12,  8,  4
};

//Returns the block at given row and column in board
char tg_get(tetris_game *obj, int row, int col){
    return obj->board[obj->cols * row + col];
}

//Set the block at the given row and column
static void tg_set(tetris_game *obj, int row, int col, char value){
    obj->board[obj->cols * row + col] =  value;
}

//Check whether the row and col are not out of bounds 
bool tg_check(tetris_game *obj, int row, int col){
    return 0 < row && row < obj->rows && 0 < col && col < obj->cols;
}

//Place a block onto the board
static void tg_put(tetris_game *obj, tetris_block block){
    int i;
    for (i = 0; i < TETRIS; i++)
    {
        tetris_location cell = TETROMINOS[block.type][block.orientation][i];
        tg_set(obj, block.location.row + cell.row , block.location.col + cell.col, TYPE_TO_CELL(block.type));
    }   
}

//Clear a block out of the board
static void tg_remove(tetris_game *obj, tetris_block block){
    int i;
    for (i = 0; i < TETRIS ; i++)
    {
        tetris_location cell = TETROMINOS[block.type][block.orientation][i];
        tg_set(obj, block.location.row + cell.row, block.location.col + cell.col, TYPE_TO_CELL(TC_EMPTY));
    }
}

//Check if a block can be placed on the board
static bool tg_fits(tetris_game *obj, tetris_block block){
    int i, r, c;
    for (i = 0; i < TETRIS; i++)
    {
        tetris_location cell = TETROMINOS[block.type][block.orientation][i];
        r = block.location.row;
        c = block.location.col;
        if(!tg_check(obj,r,c) || TC_IS_FILLED(tg_get(obj,r,c))){
            return false;
        }
    }
    return true;
}

//Return a random tetromino type
int random_tetromino(void){
    return rand() % NUM_TETROMINOS;
}

//Create a new falling block and populate the next falling block with a random one
static void tg_new_falling(tetris_game *obj){
    obj->falling = obj->next;
    obj->next.type = random_tetromino();
    obj->next.orientation = 0;
    obj->next.location.row = 0;
    obj->next.location.col = obj->cols/2 - 2;
}

//Tick gravity and move the block down if gravity should act
static void tg_do_gravity_check(tetris_game *obj){
    obj->ticks_til_gravity--;
    if (obj->ticks_til_gravity <= 0)
    {
        obj->falling.location.row++;
        tg_remove(obj, obj->falling);
        if(tg_fits(obj, obj->falling)){
            obj->ticks_til_gravity = GRAVITY_LEVEL[obj->level];
        } else {
            obj->falling.location.row--;
            tg_put(obj, obj->falling);

            tg_new_falling(obj);
        }
        tg_put(obj,obj->falling);
    }
    
}
