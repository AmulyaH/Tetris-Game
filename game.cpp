//#pragma once
#include "game.hpp"
#include "shape.hpp"
#include <iostream>

using namespace std;

Game::Game()
{
    Gamedata _data = {};
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        cerr << "Unable to initialize SDL:" << SDL_GetError() << endl;
    }

    SDL_Window *window = SDL_CreateWindow("Tetris",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          _boardWidth,
                                          _boardHeight,
                                          0);

    SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    

    _currentShape = _shapes[rand() % 7];
    _nextShape = _shapes[rand() % 7];
}

void Game::initializeGame()
{
    _level = 1;
    _score = 0;
    _isGameOver = false;
}


bool isCollision(Gamedata data, Shape current_shape)
{
    for (int x = 0; x < current_shape._size ; ++x)
    {
        for (int y = 0; y < current_shape._size; ++y)
        {
            if (current_shape.isBlock(current_shape,x,y))
            {
                int wx = current_shape._x + x;
                int wy = current_shape._y + y;
                if (wx < 0 || wx >= WIDTH || wy < 0 || wy >= BOARDHEIGHT)
                {
                    return true;
                }
                if (data.board[wy][wx].isFilled)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Game::gameRunning()
{

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            _isRunning = false;
        }
    }

    if (SDL_WaitEventTimeout(&event, 250))
    {
        Shape temp = _currentShape;
        switch (event.type)
        {
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
             
            case SDLK_DOWN:
            {
                moveShape(&temp, "down");
                if (!isCollision(_data, temp))
                {
                    moveShape(&_currentShape, "down");
                }
            }
            break;
            case SDLK_RIGHT:
            {
                //Shape temp = _currentShape;
                moveShape(&temp, "right");
                if (!isCollision(_data, temp))
                {
                    moveShape(&_currentShape, "right");
                }
            }
            break;
            case SDLK_LEFT:
            {
                //Shape temp = _currentShape;
                moveShape(&temp, "left");
                if (!isCollision(_data, temp))
                {
                    moveShape(&_currentShape, "left");
                }
            }
            break;
            case SDLK_UP:
            {
                if (!isCollision(_data, _currentShape))
                {
                    rotateShape(&_currentShape);
                }
            }
            break;
            case SDLK_SPACE:
            {
                if (_isPaused)
                {
                    _isPaused = false;
                }
                else
                {
                    _isPaused = true;
                }
            }
            break;
            case SDLK_RETURN:
            {
                initializeGame();
            }
            break;
            }
        }
        break;
        case SDL_QUIT:
            return false;
        }
    }

    return true;
}


