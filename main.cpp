//#pragma once
#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "game.hpp"

using namespace std;

int main(int argc, char *args)
{
    
    Game game;
   // bool _isRunning 
    while (game.gameRunning());
    return 0;
}
