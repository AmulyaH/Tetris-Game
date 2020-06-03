#pragma once

#include<vector>
#include "SDL2/SDL.h"
#include "shape.hpp"
#include "board.hpp"

class Game
{
private:
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
    Gamedata _data = {};

    Shape _currentShape;
    Shape _nextShape; 
    vector<Shape> _shapes;

public:
    Game();
    Gamedata initializeGame();
    bool gameRunning();
    bool isCollision(Gamedata _data, Shape _currentShape);
    void renderScore(SDL_Renderer *renderer, int score);
    void renderLevel(SDL_Renderer *renderer, int level);
    void renderText(SDL_Renderer *renderer, const char *text, int x, int y, int width, int height, SDL_Color color);
    void createShapes();
};

  



