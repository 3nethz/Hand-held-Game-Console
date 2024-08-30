#ifndef _GAME_
#define _GAME_

#include "Board.h"
#include "Piece.h"
#include "time.h"
#include "IO.h"
#include <cstdlib>

class Game
{
public:
    Game(Board *pBoard, Piece *pPiece, /*IO *pIO*/ int pScreenHeight);

    void drawScene();
    void createNewPiece();

    int mPositionX, mPositionY;
    int mPiece, mRotation;

private:
    int mScreenHeigt;
    int mNextPositionX, mNextPositionY;
    int mNextPiece, mNextRotation;

    Board *mBoard;
    Piece *mPieces;
    // IO *mIO;

    int getRand(int pA, int pB);
    void initGame();
    void drawPiece(int pY, int pX, int pPiece, int pRotation);
    void drawBoard();
};

int Game::getRand(int pA, int pB)
{
    return rand() % (pB - pA + 1) + pA;
}

void Game::initGame()
{
    srand((unsigned int)time(NULL));

    mPiece = getRand(0, 6);
    mRotation = getRand(0, 3);
    mPositionX = (BOARD_WIDTH / 2) + mPieces->getXInitialPosition(mPiece, mRotation);
    mPositionY = mPieces->getYInitialPosition(mPiece, mRotation);

    mNextPiece = getRand(0, 6);
    mNextRotation = getRand(0, 3);
    mNextPositionX = BOARD_WIDTH + 5;
    mNextPositionY = 5;
}

void Game::createNewPiece()
{
    mPiece = mNextPiece;
    mRotation = mNextRotation;
    mPositionX = (BOARD_WIDTH / 2) + mPieces->getXInitialPosition(mPiece, mRotation);
    mPositionY = mPieces->getYInitialPosition(mPiece, mRotation);

    mNextPiece = getRand(0, 6);
    mNextRotation = getRand(0, 3);
}

void Game::drawPiece(int pY, int pX, int pPiece, int pRotation)
{
    color mColor;

    int mPixelsX = mBoard->getXPositionInPixels(pX);
    int mPixelsY = mBoard->getYPositionInPixels(pY);

    for (int i = 0; i < PIECE_BLOCKS; i++)
    {
        for (int j = 0; j < PIECE_BLOCKS; j++)
        {
            switch (mPieces->getBlockType(pPiece, pRotation, pX, pY))
            {
            case 1:
                mColor = GREEN;
                break;

            case 2:
                mColor = BLUE;
                break;
            }
            //TO DO//
        }
    }
}

void Game::drawBoard(){
    int mX1 = BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH/2)) - 1;
    int mX2 = BOARD_POSITION + (BLOCK_SIZE *(BOARD_WIDTH/2));
    int mY = mScreenHeigt - (BLOCK_SIZE * BOARD_HEIGHT);
}

void Game::drawScene(){
    drawBoard();
    drawPiece(mPositionX, mPositionY, mPiece, mRotation);
    drawPiece(mNextPositionX, mNextPositionY, mNextPiece, mNextRotation);
}
#endif // _GAME_