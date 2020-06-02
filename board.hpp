#pragma once
#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "game.hpp"


struct BoardValue
{
    SDL_Color color;
    bool isFilled;
};

class Board
{
    public :
        Board();
        void renderBoard(Gamedata *data, SDL_Renderer *renderer);
};

