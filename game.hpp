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
    //Game(const Game &);
    //Game &operator=(const Game &);
    SDL_Window *_window = NULL;
    SDL_Renderer *_renderer = NULL;
    int _boardWidth = DIM * BOARDWIDTH;
    int _boardHeight = DIM * BOARDHEIGHT;
    int _level;
    int _score;
    bool _isRunning = true;
    bool _isPaused = false;
    bool _isGameOver = false;
    int _currentTimeTick = 0;
    int _lastTimeTick = 0;
    Gamedata _data;

    Shape _currentShape;
    Shape _nextShape;

public:
    Game();
    void initializeGame();
    bool gameRunning();

    bool isCollision(Gamedata _data, Shape _currentShape);
    void renderScore(SDL_Renderer *renderer, int score);
    void rencderLevel(SDL_Renderer *renderer, int level);
    void renderText(SDL_Renderer *renderer, const char *text, int x, int y, int width, int height, SDL_Color color);
};

  



