#pragma once
#include<vector>
#include "SDL2/SDL.h"
#include "shape.hpp"
#include "board.hpp"

/**
 * Game class where the game logic is written
 */ 
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
    // constructor
    Game();

    // To initialize the gamedata
    Gamedata initializeGame();

    // game running logic when playing the game
    bool gameRunning();

    // to find if the current shapes has collision with board or other shapes
    bool isCollision(Gamedata _data, Shape _currentShape);

    // to render the game score
    void renderScore(SDL_Renderer *renderer, int score);

    // to render the game level
    void renderLevel(SDL_Renderer *renderer, int level);

    // to render the text message or score or level etc
    void renderText(SDL_Renderer *renderer, const char *text, int x, int y, int width, int height, SDL_Color color, int fontSize);

    // to create the shapes required to play tetris game
    void createShapes();

    // to render/display the key and action information
    void renderLegends(SDL_Renderer *renderer);

    // to claer the shapes when game gets over
    void gameOverRender(SDL_Renderer *renderer);
};

  



