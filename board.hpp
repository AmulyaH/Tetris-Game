#pragma once
#include "SDL2/SDL.h"

#define WIDTH 10 
#define HEIGHT 20
#define BOARDHEIGHT 22
#define BOARDWIDTH 20
#define DIM 30

/// Struct to hold the data to fill the borad with color and shapes
struct BoardValue
{
    SDL_Color color;
    bool isFilled;
};

/// struct to hold the data of BoardValue to define the board with height and width
struct Gamedata
{
    BoardValue board[BOARDHEIGHT][WIDTH];
};

/**
 *  Class Board : the space to play game
 */ 
class Board
{
    public :
        Board();
        void renderBoard(Gamedata *data, SDL_Renderer *renderer);
};

