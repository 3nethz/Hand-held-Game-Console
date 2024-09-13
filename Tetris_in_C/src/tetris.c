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
    return 0 <= row && row < obj->rows && 0 <= col && col < obj->cols;
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
        tg_set(obj, block.location.row + cell.row, block.location.col + cell.col, (TC_EMPTY));
    }
}

//Check if a block can be placed on the board
static bool tg_fits(tetris_game *obj, tetris_block block){
    int i, r, c;
    for (i = 0; i < TETRIS; i++)
    {
        tetris_location cell = TETROMINOS[block.type][block.orientation][i];
        r = block.location.row + cell.row;
        c = block.location.col + cell.col;
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

/*Game move helper functions*/

//Tick gravity and move the block down if gravity should act
static void tg_do_gravity_check(tetris_game *obj){
    obj->ticks_til_gravity--;
    if (obj->ticks_til_gravity <= 0)
    {
        tg_remove(obj, obj->falling);
        obj->falling.location.row++;
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

//Move the piece left(-1) or right(+1) based on input
static void tg_move(tetris_game *obj, int direction){
    tg_remove(obj, obj->falling);
    obj->falling.location.col += direction;
    if(!tg_fits(obj, obj->falling)){
        obj->falling.location.col -= direction;
    }
    tg_put(obj,obj->falling);
}

//Send the falling block to the bottom
static void tg_down(tetris_game *obj){
    tg_remove(obj,obj->falling);
    while (tg_fits(obj,obj->falling))
    {
        obj->falling.location.row++;
    }
    obj->falling.location.row--;
    tg_put(obj,obj->falling);
    tg_new_falling(obj);
}

//Rotate the falling block in either (+1/-1) direction
static void tg_rotate(tetris_game *obj, int direction){
    tg_remove(obj, obj->falling);
    while (true)
    {
        obj->falling.orientation = (obj->falling.orientation + direction) % NUM_ORIENTATIONS;

        if(tg_fits(obj, obj->falling))
            break;

        obj->falling.location.col--;
        if(tg_fits(obj,obj->falling))
            break;

        obj->falling.location.col += 2;
        if (tg_fits(obj,obj->falling))
            break;
        
        obj->falling.location.col--;
    }
    tg_put(obj,obj->falling);
}

//Swap the falling block with the block in the hold buffer
static void tg_hold(tetris_game *obj){
    tg_remove(obj, obj->falling);
    if(obj->stored.type == -1){
        obj->stored = obj->falling;
        tg_new_falling(obj);
    } else {
        int type = obj->falling.type, orientation = obj->falling.orientation;
        obj->falling.type = obj->stored.type;
        obj->falling.orientation = obj->stored.orientation;
        obj->stored.type = type;
        obj->stored.orientation = orientation;
        while (!tg_fits(obj,obj->falling))
        {
            obj->falling.location.row--;
        }
    }
    tg_put(obj, obj->falling);
}

//Perform the action specified by the move
static void tg_handle_move(tetris_game *obj, tetris_move move){
    switch (move)
    {
    case TM_LEFT:
        tg_move(obj, -1);
        break;

    case TM_RIGHT:
        tg_move(obj, +1);
        break;
    
    case TM_DROP:
        tg_down(obj);
        break;

    case TM_CLOCK:
        tg_rotate(obj, +1);
        break;

    case TM_COUNTER:
        tg_rotate(obj, -1);
        break;

    case TM_HOLD:
        tg_hold(obj);
        break;

    default:
        //pass
        break;
    }
}

//Return true if line i is full
static bool tg_line_full(tetris_game *obj, int i){
    int j;
    for(j=0; j < obj->cols; j++){
        if(TC_IS_EMPTY(tg_get(obj,i,j)))
            return false;
    }
    return true;
}

//Shift every row above r down one
static void tg_shift_lines(tetris_game *obj, int r){
    int i,j;
    for(i= r-1; i >=0;  i--){
        for(j=0; j <obj->cols; j++){
            tg_set(obj, i+1, j, tg_get(obj,i,j));
            tg_set(obj,i,j,TC_EMPTY);
        }
    }
}

//Find rows that are filled, remove them,shift and return the number of cleared rows
static int tg_check_lines(tetris_game *obj){
    int i,nlines = 0;
    tg_remove(obj, obj->falling);
    for (i = obj->rows -1; i>=0; i--){
        if(tg_line_full(obj,i)){
            tg_shift_lines(obj,i);
            i++;
            nlines++;
        }
    }
    tg_put(obj, obj->falling);
    return nlines;
}

//Adjust the score of the game given how many lines were cleared
static void tg_adjust_score(tetris_game *obj, int lines_cleared){
    static int line_multiplier[] = {0, 40, 100, 300, 1200};
    obj->points += line_multiplier[lines_cleared] * (obj->level + 1);
    if(lines_cleared >= obj->lines_remaining){
        obj->level = MIN(MAX_LEVEL, obj->level + 1);
        lines_cleared -= obj->lines_remaining;
        obj->lines_remaining = LINES_PER_LEVEL - lines_cleared;
    } else {
        obj->lines_remaining -= lines_cleared;
    }
}

//Returns true if game over
static bool tg_game_over(tetris_game *obj){
    int i,j;
    bool over = false;
    tg_remove(obj, obj->falling);
    for (i = 0 ; i < 2 ; i++){
        for(j = 0; j < obj->cols; j++){
            if (TC_IS_FILLED(tg_get(obj, i,j)))
                over = true;
        }
    }
    tg_put(obj, obj->falling);
    return over;
}

/*Main Public functions*/

/*Do a single game tick: process gravity,user input and score. Return true 
if the game is still running, false if over.*/
bool tg_tick(tetris_game *obj, tetris_move move){
    int lines_cleared;

    //hanlde gravity
    tg_do_gravity_check(obj);

    //handle user input
    tg_handle_move(obj, move);

    //handle score
    lines_cleared = tg_check_lines(obj);
    tg_adjust_score(obj, lines_cleared);

    //output whether the game will continue
    return !tg_game_over(obj);
}

void tg_init(tetris_game *obj,int rows,int cols){
    //initialization logic
    obj->rows = rows;
    obj->cols = cols;
    obj->board = (char*)malloc(rows * cols);
    memset(obj->board, TC_EMPTY, rows * cols);
    obj->level = 0;
    obj->points = 0;
    obj->ticks_til_gravity = GRAVITY_LEVEL[obj->level];
    obj->lines_remaining = LINES_PER_LEVEL;
    srand((time(NULL)));
    tg_new_falling(obj);
    tg_new_falling(obj);
    obj->stored.type = -1;
    obj->stored.orientation = 0;
    obj->stored.location.row = 0;
    obj->next.location.col = obj->cols/2 -2;
    printf("%d", obj->falling.location.col);
}

tetris_game *tg_create(int rows, int cols){
    tetris_game *obj = (tetris_game*)malloc(sizeof(tetris_game));
    tg_init(obj, rows, cols);
    return obj;
}

void tg_destroy(tetris_game *obj){
    free(obj->board);
}

void tg_delete(tetris_game *obj){
    tg_destroy(obj);
    free(obj);
}

tetris_game *tg_load(FILE *f){
    tetris_game *obj = (tetris_game*)malloc(sizeof(tetris_game));
    fread(obj,sizeof(tetris_game), 1, f);
    obj->board = (char*)malloc(obj->rows * obj->cols);
    fread(obj->board,sizeof(char), obj->rows * obj->cols, f);
    return obj;
}

//Saves the game to a file
void tg_save(tetris_game *obj, FILE *f){
    fwrite(obj,sizeof(tetris_game),1, f);
    fwrite(obj->board,sizeof(char),obj->rows * obj->cols, f);
}

void tg_print(tetris_game *obj, FILE *f){
    int i,j;
    for(i = 0; i < obj->rows; i++){
        for (j = 0; j< obj->cols; j++){
            if(TC_IS_EMPTY(tg_get(obj,i,j))){
                fputs(TC_EMPTY_STR,f);
            } else {
                fputs(TC_BLOCK_STR,f);
            }
        }
        fputc('\n', f);
    }
}