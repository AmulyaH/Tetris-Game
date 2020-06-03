#include "game.hpp"
#include <iostream>

using namespace std;


/// Game class constructor
Game::Game()
{
    // initializting the SDL 
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        cerr << "Unable to initialize SDL:" << SDL_GetError() << endl;
    }

    // Initializing the ttf for font
    if (TTF_Init() == -1)
        {
            cerr << "Error ." << endl;
        }

    // Initializing the SDL window for Tetris game
    _window = SDL_CreateWindow("Tetris",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               _boardWidth,
                               _boardHeight,
                               0);

    // Initializing the Renderer used to render board, shape and text to the SDL window
    _renderer = SDL_CreateRenderer(_window,
                                   -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Gamedata _data = initializeGame();
    
}


// 
/**
 * Method to initialize the Tetromino shapes required for tetris game
 * The 7 required shapes
 */ 
void Game::createShapes()
{
    bool shape1[4][4] = {0, 0, 1, 0, // L
                         1, 1, 1, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0};

    bool shape2[4][4] = {1, 1, 0, 0, // Z
                         0, 1, 1, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0};

    bool shape3[4][4] = {1, 1, 1, 1, // I
                         0, 0, 0, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0};

    bool shape4[4][4] = {1, 0, 0, 0, // J
                         1, 1, 1, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0};

    bool shape5[4][4] = {1, 1, 0, 0, // 0
                         1, 1, 0, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0};

    bool shape6[4][4] = {0, 1, 1, 0, // S
                         1, 1, 0, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0};

    bool shape7[4][4] = {0, 1, 0, 0, // T
                         1, 1, 1, 0,
                         0, 0, 0, 0,
                         0, 0, 0, 0};

    // Pushing all the shapes into a vector 
    _shapes.push_back(Shape(SDL_Color{255, 0, 0}, shape1, 0, 0, 3));
    _shapes.push_back(Shape(SDL_Color{0, 255, 0}, shape2, 0, 0, 3));
    _shapes.push_back(Shape(SDL_Color{0, 0, 255}, shape3, 0, 0, 4));
    _shapes.push_back(Shape(SDL_Color{255, 255, 0}, shape4, 0, 0, 3));
    _shapes.push_back(Shape(SDL_Color{0, 255, 255}, shape5, 0, 0, 3));
    _shapes.push_back(Shape(SDL_Color{255, 0, 255}, shape6, 0, 0, 3));
    _shapes.push_back(Shape(SDL_Color{255, 255, 255}, shape7, 0, 0, 3));
}

/**
 * The initializer : creates the shapes, init the Gamedata etc required to start game from fresh
 * @return Gamedata
 */ 
Gamedata Game::initializeGame()
{
    createShapes();
    _level = 1;
    _score = 0;
    _isGameOver = false;
    Gamedata _data = {};
    _currentShape = _shapes[rand() % 7];
    _nextShape = _shapes[rand() % 7];
    _currentTimeTick = SDL_GetTicks();

    return _data;
}

/**
 * method to render/display the key and action information
 * @param renderer SDL renderer to render the text
 */ 
void Game::renderLegends(SDL_Renderer *renderer)
{
    SDL_Color White = {255, 255, 255, 255};
    
    renderText(renderer, "PgUp : Rotate Shape", 11, 0, (7 * DIM), 30, White, 8);
    renderText(renderer, "Home : Move Right", 11, 1, (7 * DIM), 30, White, 8);
    renderText(renderer, "END : Move Left", 11, 2, (7 * DIM), 30, White, 8);
    renderText(renderer, "PgDN : Move Down", 11, 3, (7 * DIM), 30, White, 8);
    renderText(renderer, "Space : Pause", 11, 4, (7 * DIM), 30, White, 8);
    renderText(renderer, "Enter : New Game", 11, 5, (7 * DIM), 30, White, 8);
    renderText(renderer, "Escape : Exit Game", 11, 6, (7 * DIM), 30, White, 8);

}

/**
 * checks if the shapes has collision with board walls or with with shapes
 * @param data gamedata conatins board information
 * @param current_shape the shape for which need to find the collision with wall or with other shapes
 * @return boolian true/false
 */ 

bool Game::isCollision(Gamedata data, Shape current_shape)
{
    // nested loop to check for each place(DIM or partition) in a shape
    for (int x = 0; x < current_shape._size; ++x)
    {
        for (int y = 0; y < current_shape._size; ++y)
        {
            if (current_shape.isBlock(current_shape, x, y))
            {
                int shapeOffsetX = current_shape._offsetX + x;
                int shapeOffsety = current_shape._offsetY + y;
                // if shape's offsets are lessthan or greater than min and max width and height of the board, 
                // return collision is true
                if (shapeOffsetX < 0 || shapeOffsetX >= WIDTH || shapeOffsety < 0 || shapeOffsety >= BOARDHEIGHT)
                {
                    return true;
                }
                // if the data already filled in the position, the current shape might have collided with existing shape,
                // then return collision is true
                if (data.board[shapeOffsety][shapeOffsetX].isFilled)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Renders the text displed in the game, to show score, level and game over texts.
 * @param renderer SDL renderer to render the text
 * @param text the text which displed/rendered on board
 * @param offsetX controls the rect's x coordinate
 * @param offsetY controls the rect's y coordinte
 * @param width controls the width of the rect
 * @param height controls the height of the rect
 * @param color color in rgb format, controls the color of the text.
 */ 
void Game::renderText(SDL_Renderer *renderer, const char *text, int offsetX, int offsetY, int width, int height, SDL_Color color, int fontSize)
{
    // Initialize the TTF
    if (TTF_Init() == -1)
        {
            cerr << "Error ." << endl;
        }

    //this opens a font style and sets a size
    TTF_Font *font = TTF_OpenFont("OpenSans-Regular.ttf", 20); 

    if (!font)
    {
        cout << "TTF_OpenFont: " << TTF_GetError() << "\n";
        font = TTF_OpenFont("OpenSans-Italic.ttf", fontSize);
    }

    //TTF_RenderText_Solid could only be used on SDL_Surface so have to create the surface first
    SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, text, color);

    //converting it into a texture
    SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); 

    //create a rect
    SDL_Rect text_rect;  
    text_rect.x = offsetX * DIM; 
    text_rect.y = offsetY * DIM; 
    text_rect.w = width; 
    text_rect.h = height;

    SDL_RenderCopy(renderer, Message, NULL, &text_rect);

    //freeing thes surface and texture
    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
}

/**
 * method to render the score of the game
 * @param renderer SDL renderer to render score
 * @param score the game score
 */ 
void Game::renderScore(SDL_Renderer *renderer, int score)
{
    SDL_Color White = {255, 255, 255, 255};
    string gameScore = "Score : " + to_string(score);
    renderText(renderer, gameScore.c_str(), 11, 9, (5 * DIM), 60, White, 20);
}

/**
 * method to render the level of the game
 * @param renderer SDL renderer to render score
 * @param level the game level
 */ 
void Game::renderLevel(SDL_Renderer *renderer, int level)
{
    SDL_Color White = {255, 255, 255, 255};
    string gameLevel = "Level : " + to_string(level);
    renderText(renderer, gameLevel.c_str(), 11, 11, (5 * DIM), 60, White, 20);
}

/**
 * renderer method to claer the shapes when game gets over
 * @param renderer SDL renderer to render the board
 */ 
void Game::gameOverRender(SDL_Renderer *renderer)
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = WIDTH * DIM;
    rect.h = BOARDHEIGHT * DIM;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

/**
 * method which hold the game logic
 */ 
bool Game::gameRunning()
{
    SDL_Event event;
    if (SDL_WaitEventTimeout(&event, 250))
    {
        Shape temp = _currentShape;
        switch (event.type)
        {
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_DOWN: // pgdown key, to move the shape faster
            {
                temp.moveShape(&temp, "down");
                if (!isCollision(_data, temp))
                {
                    _currentShape = temp;
                }
            }
            break;
            case SDLK_RIGHT: // right key to move the shapes to right side
            {
                temp.moveShape(&temp, "right");
                if (!isCollision(_data, temp))
                {
                    _currentShape = temp;
                }
            }
            break;
            case SDLK_LEFT: // left key to move the shapes to left side
            {
                temp.moveShape(&temp, "left");
                if (!isCollision(_data, temp))
                {
                    _currentShape = temp;
                }
            }
            break;
            case SDLK_UP:  // up key to rotate the shapes 
            {
                temp.rotateShape(&temp);
                if (!isCollision(_data, temp))
                {
                    _currentShape = temp;
                }
            }
            break;
            case SDLK_SPACE: // space key to pause the game
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
            case SDLK_RETURN: // enter key to restart the game
            {
                _data = initializeGame();
            }
            break;
            case SDLK_ESCAPE: // escape key to close the game
            {
                return false;
            }
            }
            
        }
        break;
        case SDL_QUIT: // on click on "x" (close) button the game board to close the game
            return false;
        }
    }

    // game logic, enter if the game is running i.e if its not paused or not game over.
    if (!_isPaused && !_isGameOver)
    {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);

        SDL_RenderClear(_renderer);

        Board board;
        board.renderBoard(&_data, _renderer); // render the game board

        renderLegends(_renderer);

        _currentShape.renderShape(_currentShape, _renderer); // render the shape

        renderScore(_renderer, _score); // render score

        renderLevel(_renderer, _level); // render level

        string nextShape = "Next Shape :";
        renderText(_renderer, nextShape.c_str(), 11, 13, (8 * DIM), 60, {255, 255, 255, 255}, 20); // render next shape
        _nextShape.renderNextShape(_nextShape, _renderer);

        // the SDL clock ticks, logic to set the spead of shapes to move down
        // the spead increases as the game score increases andlevel also gets increases
        // level 5 is the max level, and speed with reachs 500 miliseconds spead
        _currentTimeTick = SDL_GetTicks();
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
            else if (_score < 100)
            {
                _lastTimeTick = _currentTimeTick + 1000;
                _level = 1;
            }

            Shape temp = _currentShape;
            temp.moveShape(&temp, "down");

            // if collision happens then save the shape at the botton of the board or on top of another shape where it collided
            if (isCollision(_data, temp))
            {
                // shave the current shape and pick the next shape and make it as current shape
                // drop from the top of the board
                temp.saveShape(&_data, _currentShape, _renderer, _score);
                _currentShape = _nextShape;
                _nextShape = _shapes[rand() % 7];

                // again it will get collied only when the board get full, then render/display the game over text
                if (isCollision(_data, _currentShape))
                {
                    SDL_Color Red = {255, 0, 0};
                    gameOverRender(_renderer);
                    renderText(_renderer, "Game Over!", 1, 4, (8 * DIM), 80, Red, 24);
                    renderText(_renderer, "Press Enter to start new game", 0, 7, (10 * DIM), 60, Red, 20);
                    renderText(_renderer, "Press Escape to exit", 1, 9, (8 * DIM), 60, Red, 20);
                    _isGameOver = true;
                    if (SDL_WaitEventTimeout(&event, 250))
                    {
                        switch (event.type)
                        {
                        case SDL_KEYDOWN:
                        {
                            switch (event.key.keysym.sym)
                            {
                            case SDLK_RETURN: // on pressing on enter key, new game gets starts
                            {
                                _isGameOver = true;
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
                _currentShape = temp;
            }
        }
    }
    else if (!_isGameOver)
    {
        string pause = "Game Paused !";
        renderText(_renderer, pause.c_str(), 11, 7, (8 * DIM), 60, {255, 0, 0, 255}, 20);
    }

    SDL_RenderPresent(_renderer);

    return true;
}
