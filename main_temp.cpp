#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define WIDTH 10
#define HEIGHT 20
#define BOARDHEIGHT 22
#define DIM 30

string left, right, up, down;
unsigned int gameTime = 0;
bool running;
int32_t score = 0;
int32_t level = 1;
bool isGameOver = true;
SDL_Color White = {255, 255, 255};

struct BoardValue
{
    SDL_Color color;
    bool isFilled;
};

struct Gamedata
{
    BoardValue board[BOARDHEIGHT][WIDTH];
};

struct Shape
{
    SDL_Color color;
    bool matrix[4][4];
    double x, y;
    int size;
};



Shape blocks[7] = {
    {{255, 0, 0}, // L
     {{0, 0, 1, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}},
     0,
     0,
     3},
    {{0, 255, 0}, // Z
     {{1, 1, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}},
     0,
     0,
     3},
    {{0, 0, 255}, // I
     {{1, 1, 1, 1},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}},
     0,
     0,
     4},
    {{255, 255, 0}, // J
     {{1, 0, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}},
     0,
     0,
     3},
    {{0, 255, 255}, // 0
     {{1, 1, 0, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}},
     0,
     0,
     2},
    {{255, 0, 255}, // S
     {{0, 1, 1, 0},
      {1, 1, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}},
     0,
     0,
     3},
    {{255, 255, 255}, // T
     {{0, 1, 0, 0},
      {1, 1, 1, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}},
     0,
     0,
     3}};

Shape current_shape, next_shape;

Gamedata initilizeGame()
{
    Gamedata data = {};

    score = 0;
    level = 1;
    isGameOver = false;
    return data;
}

void RenderText(SDL_Renderer *renderer, const char *text, int x, int y, int width, int height, SDL_Color color)
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

void RenderBoard(Gamedata *data, SDL_Renderer *renderer)
{

    for (auto y = 0; y < BOARDHEIGHT; ++y)
    {
        for (auto x = 0; x < WIDTH; ++x)
        {
            BoardValue value = data->board[y][x];
            SDL_Rect rect;
            rect.x = x * DIM;
            rect.y = y * DIM;
            rect.w = DIM;
            rect.h = DIM;

            if (value.isFilled)
            {

                SDL_SetRenderDrawColor(renderer, value.color.r, value.color.g, value.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
            else
            {
                rect.x = 0;
                rect.y = 0;
                rect.w = WIDTH * DIM;
                rect.h = BOARDHEIGHT * DIM;

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

void gameOverRender(SDL_Renderer *renderer)
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = WIDTH * DIM;
    rect.h = BOARDHEIGHT * DIM;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void rotateShape(Shape *current_shape)
{
    Shape temp = *current_shape;
    for (int i = 0; i < temp.size; i++)
    {
        for (int j = 0; j < temp.size; j++)
        {
            current_shape->matrix[i][j] = temp.matrix[j][i];
        }
    }

    temp = *current_shape;
    for (int i = 0; i < temp.size; i++)
    {
        for (int j = 0; j < temp.size / 2; j++)
        {
            bool t = temp.matrix[i][j];
            current_shape->matrix[i][j] = temp.matrix[i][temp.size - j - 1];
            current_shape->matrix[i][temp.size - j - 1] = t;
        }
    }
}

SDL_Rect rect;
void RenderShape(Shape s, SDL_Renderer *renderer)
{
    for (int i = 0; i < s.size; i++)
    {
        for (int j = 0; j < s.size; j++)
        {
            if (s.matrix[i][j])
            {
                rect.x = (s.x + i) * DIM;
                rect.y = (s.y + j) * DIM;
                rect.w = DIM;
                rect.h = DIM;
                SDL_SetRenderDrawColor(renderer, s.color.r, s.color.g, s.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

void RenderNextShape(Shape nextShape, SDL_Renderer *renderer)
{
    for (int i = 0; i < nextShape.size; i++)
    {
        for (int j = 0; j < nextShape.size; j++)
        {
            if (nextShape.matrix[i][j])
            {
                rect.x = (13 + i) * DIM;
                rect.y = (12 + j) * DIM;
                rect.w = DIM;
                rect.h = DIM;

                RenderText(renderer, "Next Shape :", 11, 9, (8 * DIM), 60, White);
                SDL_SetRenderDrawColor(renderer, nextShape.color.r, nextShape.color.g, nextShape.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

void move(Shape *current_shape, string position)
{
    if (position == "down")
        current_shape->y++;

    if (position == "left")
        current_shape->x--;

    if (position == "right")
        current_shape->x++;
}

bool isBlock(Shape current_shape, int x, int y)
{
    return current_shape.matrix[x][y];
}

bool isValidCollision(Gamedata data, Shape shape)
{
    for (int x = 0; x < current_shape.size; ++x)
    {
        for (int y = 0; y < current_shape.size; ++y)
        {
            if (isBlock(current_shape, x, y))
            {
                int wx = current_shape.x + x;
                int wy = current_shape.y + y;
                if (wy < 0 || wy >= BOARDHEIGHT)
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

bool isCollision(Gamedata data, Shape current_shape)
{
    for (int x = 0; x < current_shape.size; ++x)
    {
        for (int y = 0; y < current_shape.size; ++y)
        {
            if (isBlock(current_shape, x, y))
            {
                int wx = current_shape.x + x;
                int wy = current_shape.y + y;
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

void saveShape(Gamedata *data, Shape current_shape, SDL_Renderer *renderer)
{
    for (auto x = 0; x < current_shape.size; ++x)
    {
        for (auto y = 0; y < current_shape.size; ++y)
        {
            if (isBlock(current_shape, x, y))
            {
                int wx = current_shape.x + x;
                int wy = current_shape.y + y;
                data->board[wy][wx].isFilled = true;
                data->board[wy][wx].color = current_shape.color;
            }
        }
    }
    for (int y = BOARDHEIGHT - 1; y >= 0; --y)
    {
        bool solid = true;
        for (int x = 0; x < WIDTH; ++x)
            if (!data->board[y][x].isFilled)
            {
                solid = false;
                break;
            }
        if (solid)
        {
            for (int yy = y - 1; yy >= 0; --yy)
                for (int x = 0; x < WIDTH; ++x)
                    data->board[yy + 1][x] = data->board[yy][x];
            for (int x = 0; x < WIDTH; ++x)
                data->board[0][x].isFilled = false;
            y++;
            score += 10;
        }
    }
}

void GameOver(SDL_Renderer *renderer, Gamedata data)
{
    SDL_Event event;
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
                data = initilizeGame();
            }
            break;
            }
            break;
        }
        }
    }
}
int main(int argc, char *args)
{

    Gamedata data = initilizeGame();



    int currentTime = 0;
    bool isPaused = false;

    //bool isGameOver = false;
    //if (!isPaused)
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
            return 1;
        }
        if (TTF_Init() == -1)
        {
            cerr << "Error ." << endl;
        }

        int width = DIM * 20;
        int height = DIM * BOARDHEIGHT;
        static int lastTime = 0;

        SDL_Window *window = SDL_CreateWindow("Tetris",
                                              SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED,
                                              width,
                                              height,
                                              0);

        SDL_Renderer *renderer = SDL_CreateRenderer(window,
                                                    -1,
                                                    SDL_RENDERER_ACCELERATED |
                                                        SDL_RENDERER_PRESENTVSYNC);

        running = true;

        current_shape = blocks[rand() % 7];

        //Gamedata data = initilizeGame();

        next_shape = blocks[rand() % 7];

        //unsigned int time = currentTime - lastTime;

        while (running)
        {

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
            }

            if (SDL_WaitEventTimeout(&event, 250))
            {
                switch (event.type)
                {
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_DOWN:
                    {
                        Shape temp = current_shape;
                        move(&temp, "down");
                        if (!isCollision(data, temp))
                        {
                            move(&current_shape, "down");
                        }
                    }
                    break;
                    case SDLK_RIGHT:
                    {
                        Shape temp = current_shape;
                        move(&temp, "right");
                        if (!isCollision(data, temp))
                        {
                            move(&current_shape, "right");
                        }
                    }
                    break;
                    case SDLK_LEFT:
                    {
                        Shape temp = current_shape;
                        move(&temp, "left");
                        if (!isCollision(data, temp))
                        {
                            move(&current_shape, "left");
                        }
                    }
                    break;
                    case SDLK_UP:
                    {
                        if (!isCollision(data, current_shape))
                        {
                            rotateShape(&current_shape);
                        }
                    }
                    break;
                    case SDLK_SPACE:
                    {
                        if (isPaused)
                        {
                            isPaused = false;
                        }
                        else
                        {
                            isPaused = true;
                        }
                    }
                    break;
                    case SDLK_RETURN:
                    {
                        data = initilizeGame();
                    }
                    break;
                    }
                }
                break;
                case SDL_QUIT:
                    return false;
                }
            }

            if (!isPaused && !isGameOver)
            {

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                SDL_RenderClear(renderer);

                RenderBoard(&data, renderer);

                RenderShape(current_shape, renderer);

                string gameScore = "Score : " + to_string(score);

                gameTime = SDL_GetTicks() / 1000;

                string gameLevel = "Level : " + to_string(level);

                RenderText(renderer, gameLevel.c_str(), 11, 3, (7 * DIM), 60, White);

                RenderText(renderer, gameScore.c_str(), 11, 5, (7 * DIM), 60, White);

                RenderNextShape(next_shape, renderer);

                currentTime = SDL_GetTicks();
                if (currentTime > lastTime)
                {
                    if (score > 500)
                    {
                        lastTime = currentTime + 500;
                        level = 5;
                    }
                    else if (score > 300)
                    {
                        lastTime = currentTime + 600;
                        level = 4;
                    }
                    else if (score > 200)
                    {
                        lastTime = currentTime + 700;
                        level = 3;
                    }
                    else if (score > 100)
                    {
                        lastTime = currentTime + 800;
                        level = 2;
                    }
                    else
                    {
                        lastTime = currentTime + 1000;
                        level = 1;
                    }

                    Shape t = current_shape;
                    move(&t, "down");

                    if (isCollision(data, t))
                    {
                        saveShape(&data, current_shape, renderer);
                        current_shape = next_shape;
                        next_shape = blocks[rand() % 7];

                        if (isCollision(data, current_shape))
                        {
                            SDL_Color Red = {255, 0, 0};
                            gameOverRender(renderer);
                            RenderText(renderer, "Game Over!", 1, 5, (9 * DIM), 60, Red);
                            RenderText(renderer, "Press Enter to start new game", 1, 7, (9 * DIM), 60, Red);
                            RenderText(renderer, "Press any key to exit", 1, 9, (9 * DIM), 60, Red);
                            isGameOver = true;
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
                                        gameTime = 0;
                                        isGameOver = true;
                                        data = initilizeGame();
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
                        current_shape = t;
                    }
                }
            }
            else if (!isGameOver)
            {

                string pause = "Game Paused !";
                RenderText(renderer, pause.c_str(), 11, 1, (9 * DIM), 60, White);
            }

            SDL_RenderPresent(renderer);
        }
    }
    return 0;
}
