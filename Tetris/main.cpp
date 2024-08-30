#include "IO.h"

int main(int argc, char* argv[])
{
    IO io;
    io.clearScreen();  // Clear the screen with black
    io.updateScreen();  // Present the cleared screen
    io.drawRectangle(100, 100, 200, 400, RED);  // Draw a red rectangle

    SDL_Delay(2000);  // Keep the window open for 2 seconds

    return 0;
}