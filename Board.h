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

    Board( Piece *pPieces, int pScreenHeight);
    
    int getXPositionInPixels(int pPos);
    int getYPositionInPixels(int pPos);
    bool isFreeBlock(int pX, int pY);
    bool isPossibleMovement(int pX, int pY, int pPiece, int pRotation);
    void storePiece(int pX, int pY, int pPiece, int pRotation);
    void deletePossibleLines();
    bool isGameOver();

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



#endif // _BOARD_