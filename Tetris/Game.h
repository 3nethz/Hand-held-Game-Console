#ifndef _GAME_
#define _GAME_

#include "Board.h"
#include "Piece.h"
#include "time.h"
#include "IO.h"
#include <cstdlib>

#define WAIT_TIME 700

class Game
{
public:
    Game(Board *pBoard, Piece *pPiece, IO *pIO, int pScreenHeight);

    void drawScene();
    void createNewPiece();

    int mPosX, mPosY;
    int mPiece, mRotation;

private:
    int mScreenHeight;
    int mNextPositionX, mNextPositionY;
    int mNextPiece, mNextRotation;

    Board *mBoard;
    Piece *mPieces;
    IO *mIO;

    int getRand(int pA, int pB);
    void initGame();
    void drawPiece(int pY, int pX, int pPiece, int pRotation);
    void drawBoard();
};

#endif