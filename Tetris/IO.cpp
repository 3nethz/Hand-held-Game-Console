#include "IO.h"
#include <iostream>
#include <cstdlib>

SDL_Color mColors[COLOR_MAX] = {
    {0, 0, 0, 255},      // black
    {255, 0, 0, 255},      // red
    {0, 255, 0, 255},      // blue
    {0, 255, 255, 255},  // CYAN
    {255, 0, 255, 255},  // MAGENTA
    {255, 255, 0, 255},  // YELLOW
    {255, 255, 255, 255} // WHITE
};

IO::IO()
{
    initGraph();
}

void IO::clearScreen()
{
    // Set the draw color to black (or any color you want to use as the background)
    SDL_SetRenderDrawColor(mRenderer, mColors[BLACK].r, mColors[BLACK].g, mColors[BLACK].b, mColors[BLACK].a);

    // Clear the screen with the chosen color
    SDL_RenderClear(mRenderer);

    // Present the updated renderer (show the cleared screen)
    SDL_RenderPresent(mRenderer);
}

void IO::drawRectangle(int pX1, int pY1, int pX2, int pY2, enum color pC)
{
    SDL_SetRenderDrawColor(mRenderer, mColors[pC].r, mColors[pC].g, mColors[pC].b, mColors[pC].a);

    SDL_Rect rect;
    rect.x = pX1;
    rect.y = pY1;
    rect.h = pY2 - pY1;
    rect.w = pX2 - pX1;

    SDL_RenderFillRect(mRenderer, &rect);
    SDL_RenderPresent(mRenderer);
}

void IO::updateScreen()
{
    SDL_RenderPresent(mRenderer);
}

void IO::initGraph()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        exit(1);
    }

    atexit(SDL_Quit);

    mWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);

    if (mWindow == nullptr)
    {
        std::cerr << "Unable to create window: " << SDL_GetError() << std::endl;
        exit(1);
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
    if (mRenderer == nullptr)
    {
        SDL_DestroyWindow(mWindow); 
        std::cerr << "Unable to create renderer" << SDL_GetError() << std::endl;
        exit(1);
    }
}

int IO::getScreenHeight()
{
    return 480;
}

int IO::pollKey()
{
    SDL_PollEvent(e){
        if (e.type == SDL_KEYDOWN)
        {
            return e.key.keysym.sym;
        }
    }
}
