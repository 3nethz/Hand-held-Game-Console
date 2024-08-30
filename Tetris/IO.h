#ifndef _IO_
#define _IO_

#include <SDL.h>
//#include "SDL_gfxPrimitives.h"

enum color{
    BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, COLOR_MAX
};

class IO {
    public:
    IO();

    void drawRectangle(int pX1, int pY1, int pX2, int pY2, enum color pC);
    void clearScreen();
    int getScreenHeight();
    void initGraph();
    int pollKey();
    int getKey();
    int isKeyDown(int pKey);
    void updateScreen();
    
    private:
    SDL_Window* mWindow = nullptr;
    SDL_Renderer* mRenderer = nullptr;

};



#endif