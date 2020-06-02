#include "board.hpp"
//#include "game.hpp"

Board::Board()
{};

void Board::renderBoard(Gamedata *data, SDL_Renderer *renderer)
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