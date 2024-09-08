#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>
#include <stdbool.h>

// converting a tetromino type to its corresponding cell
#define TYPE_TO_CELL(x) ((x) + 1)

// strings for printing a tetris board
#define TC_EMPTY_STR " "
#define TC_BLOCK_STR "\u2588"

// checking tetris cell
#define TC_IS_EMPTY(X) ((X) == TC_EMPTY)
#define TC_IS_FILLED(X) (!TC_IS_EMPTY(X))

// No of cells in a tetromino
#define TETRIS 4

// No of Tetrominos
#define NUM_TETROMINOS 7

// No of orientations for a tetromino
#define NUM_ORIENTATIONS 4

// No of levels
#define MAX_LEVEL 19
#define LINES_PER_LEVEL 10

// cell is a 1x1 block withing a tetris board
typedef enum
{
    TC_EMPTY,
    TC_CELLI,
    TC_CELLL,
    TC_CELLJ,
    TC_CELLO,
    TC_CELLS,
    TC_CELLT,
    TC_CELLZ
} tetris_cell;

// type of the shapes without the orientation
typedef enum
{
    TET_I,
    TET_L,
    TET_J,
    TET_S,
    TET_O,
    TET_T,
    TET_Z
} tetris_type;

// a row column pair. negative numbers allowed for offset
typedef struct
{
    int row;
    int col;
} tetris_location;

// block is a struct that contains info about a tetromino. its type, orientation and location
typedef struct
{
    int type;
    int orientation;
    tetris_location location;
} tetris_block;

// all possible inputs to the game
typedef enum
{
    TM_LEFT,
    TM_RIGHT,
    TM_CLOCK,
    TM_COUNTER,
    TM_HOLD,
    TM_NONE
} tetris_move;

// a game object

typedef struct
{
    // game board properties
    int rows;
    int cols;
    char *board;

    // scoring information
    int points;
    int level;

    // falling block is the currently going down one, next is the next in line, stored is the block that you can swap out
    tetris_block falling;
    tetris_block next;
    tetris_block stored;

    int ticks_til_gravity;
    int lines_remaining;
} tetris_game;

/*this array stores info about the cells that are filled by each tetromino.
The first index is the type of tetromino(I,O,L,S,Z,J,T).
The second index is the orientation(0-3).The final array contains 4 tetris
location objects each mapping to an offset from the point which is the
upper left of that is the tetromino origin*/
extern tetris_location TETROMINOS[NUM_TETROMINOS][NUM_ORIENTATIONS][TETRIS];

/*this array tells how many ticks per gravity level*/
extern int GRAVITY_LEVEL[MAX_LEVEL + 1];

// Data structure manipulation
void tg_init(tetris_game *obj, int rows, int cols);
tetris_game *tg_create(int rows, int cols);
void tg_destroy(tetris_game *obj);
void tg_delete(tetris_game *obj);
tetris_game *tg_load(FILE *f);
void tg_save(tetris_game *obj, FILE *f);

// public methods not related to memory
char tg_get(tetris_game *obj, int row, int col);
bool tg_check(tetris_game *obj, int row, int col);
bool tg_tick(tetris_game *obj, tetris_move move);
void tg_print(tetris_game *obj, FILE *f);

#endif // TETRIS_H
