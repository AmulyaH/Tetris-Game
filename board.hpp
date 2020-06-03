#pragma once
#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
//#include "game.hpp"


#define WIDTH 10
#define HEIGHT 20
#define BOARDHEIGHT 22
#define BOARDWIDTH 20
#define DIM 30


/* struct BoardValue
{
    SDL_Color color;
    bool isFilled;
}; */

struct BoardValue
{
    SDL_Color color;
    bool isFilled;
};
struct Gamedata
{
    BoardValue board[BOARDHEIGHT][WIDTH];
};

class Board
{
    public :
        Board();
        void renderBoard(Gamedata *data, SDL_Renderer *renderer);
};

