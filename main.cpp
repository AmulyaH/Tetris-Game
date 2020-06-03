
/* #include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>

#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h> */
//#include <iostream>
#include "game.hpp"

using namespace std;

int main(int argc, char **args)
{
    Game game;
    // looping through till game gets over or user choose to close the game or exit by pressing escape button
    while (game.gameRunning()); 
    return 0;
}
