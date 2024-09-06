#ifndef _PIECE_
#define _PIECE_

class Piece
{
public:
    Piece();

    int getBlockType(int pPiece, int pRotation, int pX, int pY);
    int getXInitialPosition(int pPiece, int pRotation);
    int getYInitialPosition(int pPiece, int pRotation);

private:
    int mPieces[7][4][5][5] = {
        // Square
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}}},

        // I
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 2, 1, 1},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {1, 1, 2, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 1, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}}},
        // L
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 2, 1, 0},
             {0, 1, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 1, 0},
             {0, 1, 2, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}},
        // L mirrored
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 1, 0, 0, 0},
             {0, 1, 2, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 2, 1, 0},
             {0, 0, 0, 1, 0},
             {0, 0, 0, 0, 0}}},
        // N
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 1, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 2, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 2, 0, 0},
             {0, 1, 0, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}},
        // N mirrored
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 0, 1, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 1, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 1, 0, 0, 0},
             {0, 1, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 1, 0},
             {0, 1, 2, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}},
        // T
        {
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 2, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 1, 2, 1, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 2, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 0, 0, 0, 0}},
            {{0, 0, 0, 0, 0},
             {0, 0, 1, 0, 0},
             {0, 1, 2, 1, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}}}};
    int mPiecesInitialPosition[7 /*kind */][4 /* r2otation */][2 /* position */] = {
        /* Square */
        {
            {-2, -3},
            {-2, -3},
            {-2, -3},
            {-2, -3}},
        /* I */
        {
            {-2, -2},
            {-2, -3},
            {-2, -2},
            {-2, -3}},
        /* L */
        {
            {-2, -3},
            {-2, -3},
            {-2, -3},
            {-2, -2}},
        /* L mirrored */
        {
            {-2, -3},
            {-2, -2},
            {-2, -3},
            {-2, -3}},
        /* N */
        {
            {-2, -3},
            {-2, -3},
            {-2, -3},
            {-2, -2}},
        /* N mirrored */
        {
            {-2, -3},
            {-2, -3},
            {-2, -3},
            {-2, -2}},
        /* T */
        {
            {-2, -3},
            {-2, -3},
            {-2, -3},
            {-2, -2}},
    };
};

#endif // _PIECE_