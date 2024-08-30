#ifndef _BOARD_
#define _BOARD_

#include "Piece.h"

#define BOARD_LINE_WIDTH 6
#define BLOCK_SIZE 16
#define BOARD_POSITION 320
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define MIN_VERTICAL_MARGIN 20
#define MIN_HORIZONTAL_MARGIN 20
#define PIECE_BLOCKS 5

class Board
{
public:
    int getXPositionInPixels(int pPos);
    int getYPositionInPixels(int pPos);
    bool isFreeBlock(int pX, int pY);
    bool isPossibleMovement(int pX, int pY, int pPiece, int pRotation);
    void storePieces(int pX, int pY, int pPiece, int pRotation);
    void deletePossibleLines();
    bool isGameOver();

private:
    enum
    {
        POS_FREE,
        POS_FILLED
    };
    int mBoard[BOARD_WIDTH][BOARD_HEIGHT];
    Piece *mPieces;
    int mScreenHeight;

    void initBoard();
    void deleteLine(int pY);
};

int Board::getXPositionInPixels(int pPos)
{
    return ((BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2))) + (pPos * BLOCK_SIZE));
}

int Board::getYPositionInPixels(int pPos)
{
    return ((BOARD_POSITION - (BLOCK_SIZE * (BOARD_HEIGHT / 2))) + (pPos * BLOCK_SIZE));
};

bool Board::isFreeBlock(int pX, int pY)
{
    return (mBoard[pX][pY] == POS_FREE);
};

bool Board::isPossibleMovement(int pX, int pY, int pPiece, int pRotation)
{
    for (int i1 = pX, i2 = 0; i1 < pX + PIECE_BLOCKS; i1++, i2++)
    {
        for (int j1 = pY, j2 = 0; j1 < pY + PIECE_BLOCKS; j1++, j2++)
        {
            if (i1 < 0 || i1 > BOARD_WIDTH - 1 || j1 > BOARD_HEIGHT - 1)
            {
                if (mPieces->getBlockType(pPiece, pRotation, j2, i2) != 0)
                {
                    return false;
                }
            }
            if (j2 >= 0)
            {
                if (mPieces->getBlockType(pPiece, pRotation, j2, i2) != 0 && isFreeBlock(i1, j1))
                {
                    return false;
                }
            }
        }
    }
    return true;
};

void Board::storePieces(int pX, int pY, int pPiece, int pRotation)
{
    for (int i1 = pX, i2; i1 < pX + BLOCK_SIZE; i1++, i2++)
    {
        for (int j1 = pY, j2 = 0; j1 < pY + BLOCK_SIZE; j1++, j2++)
        {
            if (mPieces->getBlockType(pPiece, pRotation, pX, pY) != 0)
            {
                mBoard[i1][j1] = POS_FILLED;
            }
        }
    }
}

void Board::deletePossibleLines()
{
    for (int j = 0; j < BOARD_HEIGHT; j++)
    {
        int i = 0;
        while (i < BOARD_WIDTH)
        {
            if (mBoard[i][j] != POS_FILLED)
                break;
        }
        if (i == BOARD_WIDTH)
        {
            deleteLine(j);
        }
    }
}

void Board::deleteLine(int pY)
{
    for (int j = pY; j > 0; j--)
    {
        for (int i = 0; i < BOARD_WIDTH; i++)
        {
            mBoard[i][j] = mBoard[i][j - 1];
        }
    }
}

void Board::initBoard()
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            mBoard[i][j] == POS_FREE;
        }
    }
}

bool Board::isGameOver()
{
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        if (mBoard[i][0] == POS_FILLED)
            return true;
    }
    return false;
}

#endif // _BOARD_