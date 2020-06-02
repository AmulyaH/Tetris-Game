#pragma once
#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "board.hpp"
#include "shape.hpp"

#define WIDTH 10
#define HEIGHT 20
#define BOARDHEIGHT 22
#define BOARDWIDTH 20
#define DIM 30


struct Gamedata
{
    BoardValue board[BOARDHEIGHT][WIDTH];
};
 
class Game
{
private:
    Game(const Game &);
    Game &operator=(const Game &);
    int _boardWidth = DIM * BOARDWIDTH;
    int _boardHeight = DIM * BOARDHEIGHT;
    int _level;
    int _score;
    bool _isRunning = true;
    bool _isPaused = false;
    bool _isGameOver = false;
    Gamedata _data;

    Shape _currentShape;
    Shape _nextShape;

public:
    Game();
    void initializeGame();
    bool gameRunning();

    bool isCollision(Gamedata _data, Shape _currentShape);
   
};

  



