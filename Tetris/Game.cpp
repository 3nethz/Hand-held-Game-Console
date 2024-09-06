#include "Game.h"
#include <iostream>

Game::Game(Board *pBoard, Piece *pPiece, IO *pIO, int pScreenHeight)
{
    mBoard = pBoard;
    mPieces = pPiece;
    mIO = pIO;
    mScreenHeight = pScreenHeight;
    initGame();
}

int Game::getRand(int pA, int pB)
{
    return rand() % (pB - pA + 1) + pA;
}

void Game::initGame()
{
    srand((unsigned int)time(NULL));

    mPiece = getRand(0, 6);
    mRotation = getRand(0, 3);
    mPosX = (BOARD_WIDTH / 2) + mPieces->getXInitialPosition(mPiece, mRotation);
    mPosY = mPieces->getYInitialPosition(mPiece, mRotation);
    mNextPiece = getRand(0, 6);
    mNextRotation = getRand(0, 3);
    mNextPositionX = BOARD_WIDTH + 5;
    mNextPositionY = 5;
}

void Game::createNewPiece()
{
    mPiece = mNextPiece;
    mRotation = mNextRotation;
    mPosX = (BOARD_WIDTH / 2) + mPieces->getXInitialPosition(mPiece, mRotation);
    mPosY = mPieces->getYInitialPosition(mPiece, mRotation);

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
            char blockType = mPieces->getBlockType(pPiece, pRotation, j, i);
            switch (blockType)
            {
            case 1:
                mColor = GREEN;
                break; // For each block of the piece except the pivot
            case 2:
                mColor = BLUE;
                break; // For the pivot
            }
            if (mPieces->getBlockType(pPiece, pRotation, j, i) != 0)
            {
                mIO->drawRectangle(mPixelsX + i * BLOCK_SIZE,
                                   mPixelsY + j * BLOCK_SIZE,
                                   (mPixelsX + i * BLOCK_SIZE) + BLOCK_SIZE - 1,
                                   (mPixelsY + j * BLOCK_SIZE) + BLOCK_SIZE - 1, mColor);
                // Calculate positions
                int posX1 = mPixelsX + i * BLOCK_SIZE;
                int posY1 = mPixelsY + j * BLOCK_SIZE;
                int posX2 = (mPixelsX + i * BLOCK_SIZE) + BLOCK_SIZE - 1;
                int posY2 = (mPixelsY + j * BLOCK_SIZE) + BLOCK_SIZE - 1;
            }
        }
    }
}

void Game::drawBoard()
{
    int mX1 = BOARD_POSITION - (BLOCK_SIZE * (BOARD_WIDTH / 2)) - 1;
    int mX2 = BOARD_POSITION + (BLOCK_SIZE * (BOARD_WIDTH / 2));
    int mY = mScreenHeight - (BLOCK_SIZE * BOARD_HEIGHT);

    mIO->drawRectangle(mX1 - BOARD_LINE_WIDTH, mY, mX1, mScreenHeight - 1, BLUE);
    mIO->drawRectangle(mX2, mY, mX2 + BOARD_LINE_WIDTH, mScreenHeight - 1, BLUE);

    mX1 += 1;
    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        for (int j = 0; j < BOARD_HEIGHT; j++)
        {
            // Check if the block is filled, if so, draw it
            if (!mBoard->isFreeBlock(i, j))
                mIO->drawRectangle(mX1 + i * BLOCK_SIZE,
                                   mY + j * BLOCK_SIZE,
                                   (mX1 + i * BLOCK_SIZE) + BLOCK_SIZE - 1,
                                   (mY + j * BLOCK_SIZE) + BLOCK_SIZE - 1,
                                   RED);
        }
    }
}

void Game::drawScene()
{
    drawBoard();
    drawPiece(mPosY, mPosX, mPiece, mRotation);
    drawPiece(mNextPositionY, mNextPositionX, mNextPiece, mNextRotation);
}