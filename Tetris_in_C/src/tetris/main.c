#include <ncurses/ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "tetris.h"
#include "util.h"

// 2 columns per cell
#define COLS_PER_CELL 2

// Macro to print a cell of a specific type to a window
#define ADD_BLOCK(w, x) waddch((w), ' ' | A_REVERSE | COLOR_PAIR(x)); \
                        waddch((w), ' ' | A_REVERSE | COLOR_PAIR(x))
#define ADD_EMPTY(w) waddch((w), ' '); waddch((w), ' ')

// Print the tetris board onto the ncurses window
void display_board(WINDOW *w, tetris_game *obj)
{
    int i, j;
    box(w, 0, 0);
    for (i = 0; i < obj->rows; i++)
    {
        wmove(w, 1 + i, 1);
        for (j = 0; j < obj->cols; j++)
        {
            if (TC_IS_FILLED(tg_get(obj, i, j)))
            {
                ADD_BLOCK(w, tg_get(obj, i, j));
            }
            else
            {
                ADD_EMPTY(w);
            }
        }
    }
    wnoutrefresh(w);
}

// Display a tetris piece in a dedicated window
void display_piece(WINDOW *w, tetris_block block)
{
    int b;
    tetris_location c;
    wclear(w);
    box(w, 0, 0);
    if (block.type == -1)
    {
        wnoutrefresh(w);
        return;
    }
    for (b = 0; b < TETRIS; b++)
    {
        c = TETROMINOS[block.type][block.orientation][b];
        wmove(w, c.row + 1, c.col * COLS_PER_CELL + 1);
        ADD_BLOCK(w, TYPE_TO_CELL(block.type));
    }
    wnoutrefresh(w);
}

// Display score information in a different window
void display_score(WINDOW *w, tetris_game *tg)
{
    wclear(w);
    box(w, 0, 0);
    wprintw(w, "Score\n%d\n", tg->points);
    wprintw(w, "Level\n%d\n", tg->level);
    wprintw(w, "Lines\n%d\n", tg->lines_remaining);
    wnoutrefresh(w);
}

void boss_mode()
{
    clear();
    printw("123");
    echo();
    timeout(-1);
    while (getch() != KEY_F(1))
        ;
    timeout(0);
    noecho();
    clear();
}

// save and exit game
void save(tetris_game *game, WINDOW *w)
{
    FILE *f;

    wclear(w);
    box(w, 0, 0); // return the border
    wmove(w, 1, 1);
    wprintw(w, "Save and exit(Y/N)");
    wrefresh(w);
    timeout(-1);
    if (getch() == 'n')
    {
        timeout(0);
        return;
    }
    f = fopen("tetris.save", "w");
    tg_save(game, f);
    fclose(f);
    tg_delete(game);
    endwin();
    printf("Game saved to \"tetris.save\".\n");
    printf("Resume by passing the filename as an argument to this program.\n");
    exit(EXIT_SUCCESS);
}

// ncurses initialization for color blocks
void init_colors(void)
{
    start_color();
    init_pair(TC_CELLI, COLOR_CYAN, COLOR_BLACK);
    init_pair(TC_CELLJ, COLOR_BLUE, COLOR_BLACK);
    init_pair(TC_CELLL, COLOR_WHITE, COLOR_BLACK);
    init_pair(TC_CELLO, COLOR_YELLOW, COLOR_BLACK);
    init_pair(TC_CELLS, COLOR_GREEN, COLOR_BLACK);
    init_pair(TC_CELLT, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(TC_CELLZ, COLOR_RED, COLOR_BLACK);
}

// main tetris game
int main(int argc, char **argv)
{
    tetris_game *tg;
    tetris_move move = TM_NONE;
    bool running = true;
    WINDOW *board, *next, *hold, *score;

    if (argc >= 2)
    {
        FILE *f = fopen(argv[1], "r");
        if (f == NULL)
        {
            perror("tetris");
            exit(EXIT_FAILURE);
        }
        tg = tg_load(f);
        fclose(f);
    }
    else
    {
        // otherwise create new game
        tg = tg_create(22, 10);
    }
    // ncurses initialization

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(0);
    curs_set(0);
    init_colors();

    // create windows for each section of the interface
    board = newwin(tg->rows + 2, 2 * tg->cols + 2, 0, 0);
    next = newwin(6, 10, 0, 2 * (tg->cols + 1) + 1);
    hold = newwin(6, 10, 7, 2 * (tg->cols + 1) + 1);
    score = newwin(6, 10, 14, 2 * (tg->cols + 1) + 1);

    while (running)
    {
        running = tg_tick(tg, move);
        display_board(board, tg);
        display_piece(next, tg->next);
        display_piece(hold, tg->stored);
        display_score(score, tg);
        doupdate();
        sleep_milli(10);

        switch (getch())
        {
        case KEY_LEFT:
            move = TM_LEFT;
            break;
        case KEY_RIGHT:
            move = TM_RIGHT;
            break;
        case KEY_UP:
            move = TM_CLOCK;
            break;
        case KEY_DOWN:
            move = TM_DROP;
            break;
        case 'q':
            running = false;
            move = TM_NONE;
            break;
        case 'p':
            wclear(board);
            box(board, 0, 0);
            wmove(board, tg->rows / 2, (tg->cols * COLS_PER_CELL - 6) / 2);
            wprintw(board, "PAUSED");
            wrefresh(board);
            timeout(-1);
            getch();
            timeout(0);
            move = TM_NONE;
            break;
        case 'b':
            boss_mode();
            move = TM_NONE;
            break;
        case 's':
            save(tg, board);
            move =  TM_NONE;
            break;
        case ' ':
            move = TM_HOLD;
            break;

        default:
            move = TM_NONE;
        }
    }

    wclear(stdscr);
    endwin();
    printf("%d",running);
    printf("Game Over!\n");
    printf("You finished with %d points on level %d. \n", tg->points, tg->level);

    tg_delete(tg);
    return(0);
}
