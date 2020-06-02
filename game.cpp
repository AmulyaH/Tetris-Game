//#pragma once
#include "game.hpp"
#include "shape.hpp"
#include "board.hpp"
#include <iostream>

using namespace std;

Game::Game()
{
    //Gamedata _data = {};

    _currentTimeTick = SDL_GetTicks();
    _lastTimeTick = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        cerr << "Unable to initialize SDL:" << SDL_GetError() << endl;
    }

    _window = SDL_CreateWindow("Tetris",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               _boardWidth,
                               _boardHeight,
                               0);

    _renderer = SDL_CreateRenderer(_window,
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
    //Gamedata _data = {};
}

bool isCollision(BoardValue board, Shape current_shape)
{
    for (int x = 0; x < current_shape._size; ++x)
    {
        for (int y = 0; y < current_shape._size; ++y)
        {
            if (current_shape.isBlock(current_shape, x, y))
            {
                int wx = current_shape._x + x;
                int wy = current_shape._y + y;
                if (wx < 0 || wx >= WIDTH || wy < 0 || wy >= BOARDHEIGHT)
                {
                    return true;
                }
                if (board[wy][wx])
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void renderText(SDL_Renderer *renderer, const char *text, int x, int y, int width, int height, SDL_Color color)
{
    TTF_Font *font;
    font = TTF_OpenFont("open-sans/OpenSans-Italic.ttf", 24); //this opens a font style and sets a size

    SDL_Color White = {255, 255, 255}; // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

    if (!font)
    {
        cout << "TTF_OpenFont: " << TTF_GetError() << "\n";
    }

    SDL_Surface *surfaceMessage;
    surfaceMessage = TTF_RenderText_Solid(font, text, color); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

    SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

    SDL_Rect Message_rect;    //create a rect
    Message_rect.x = x * DIM; //controls the rect's x coordinate
    Message_rect.y = y * DIM; // controls the rect's y coordinte
    Message_rect.w = width;   // controls the width of the rect
    Message_rect.h = height;  // controls the height of the rect

    //Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understand

    //Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

    SDL_RenderCopy(renderer, Message, NULL, &Message_rect); //you put the renderer's name first, the Message, the crop size(you can ignore this if you don't want to dabble with cropping), and the rect which is the size and coordinate of your texture

    //Don't forget to free your surface and texture
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

void renderScore(SDL_Renderer *renderer, int score)
{
    SDL_Color White = {255, 255, 255, 255};
    //SDL_Color Red = {255, 0,0,255};
    string gameScore = "Score : " + to_string(score);
    renderText(renderer, gameScore.c_str(), 11, 3, (7 * DIM), 60, White);
}

void renderLevel(SDL_Renderer *renderer, int level)
{
    SDL_Color White = {255, 255, 255, 255};
    //SDL_Color Red = {255, 0,0,255};
    string gameLevel = "Score : " + to_string(level);
    renderText(renderer, gameLevel.c_str(), 11, 3, (7 * DIM), 60, White);
}

bool Game::gameRunning()
{

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return false;
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
                temp.moveShape(&temp, "down");
                if (!isCollision(_data, temp))
                {
                    temp.moveShape(&_currentShape, "down");
                }
            }
            break;
            case SDLK_RIGHT:
            {
                //Shape temp = _currentShape;
                temp.moveShape(&temp, "right");
                if (!isCollision(_data, temp))
                {
                    temp.moveShape(&_currentShape, "right");
                }
            }
            break;
            case SDLK_LEFT:
            {
                //Shape temp = _currentShape;
                temp.moveShape(&temp, "left");
                if (!isCollision(_data, temp))
                {
                    temp.moveShape(&_currentShape, "left");
                }
            }
            break;
            case SDLK_UP:
            {
                if (!isCollision(_data, _currentShape))
                {
                    temp.rotateShape(&_currentShape);
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

    if (!_isPaused && !_isGameOver)
    {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);

        SDL_RenderClear(_renderer);

        Board b;
        b.renderBoard(&_data, _renderer);

        _currentShape.renderShape(_currentShape, _renderer);

        renderScore(_renderer, _score);

        rencderLevel(_renderer, _level);

        _nextShape.renderNextShape(_nextShape, _renderer);

        // _currentTimeTick = SDL_GetTicks();
        if (_currentTimeTick > _lastTimeTick)
        {
            if (_score > 500)
            {
                _lastTimeTick = _currentTimeTick + 500;
                _level = 5;
            }
            else if (_score > 300)
            {
                _lastTimeTick = _currentTimeTick + 600;
                _level = 4;
            }
            else if (_score > 200)
            {
                _lastTimeTick = _currentTimeTick + 700;
                _level = 3;
            }
            else if (_score > 100)
            {
                _lastTimeTick = _currentTimeTick + 800;
                _level = 2;
            }
            else
            {
                _lastTimeTick = _currentTimeTick + 1000;
                _level = 1;
            }
        }

        Shape t = _currentShape;
        t.moveShape(&t, "down");

        if (isCollision(_data, t))
        {
            t.saveShape(&_data, _currentShape, _renderer, _score);
            _currentShape = _nextShape;
            _nextShape = _shapes[rand() % 7];

            if (isCollision(_data, _currentShape))
            {
                SDL_Color Red = {255, 0, 0};
                //gameOverRender(renderer);
                renderText(_renderer, "Game Over!", 1, 5, (9 * DIM), 60, Red);
                renderText(_renderer, "Press Enter to start new game", 1, 7, (9 * DIM), 60, Red);
                renderText(_renderer, "Press any key to exit", 1, 9, (9 * DIM), 60, Red);
                _isGameOver = true;
                if (SDL_WaitEventTimeout(&event, 250))
                {
                    switch (event.type)
                    {
                    case SDL_KEYDOWN:
                    {
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_RETURN:
                        {
                            //gameTime = 0;
                            _isGameOver = true;
                            _data = initializeGame();
                        }
                        break;
                        }
                        break;
                    }
                    }
                }
            }
        }
        else
        {
            _currentShape = t;
        }
    }
    else if (!_isGameOver)
    {
        string pause = "Game Paused !";
        renderText(_renderer, pause.c_str(), 11, 1, (9 * DIM), 60, {255, 255, 255, 255});
    }

    SDL_RenderPresent(_renderer);

    return true;
}
