
#include "board.hpp"

/// Default constructor
Board::Board()
{};

/**
 * Renders the game board
 * @param data Game data details 
 * @param renderer SDL renderer to render the board
 */
void Board::renderBoard(Gamedata *data, SDL_Renderer *renderer)
{   
    for (auto y = 0; y < BOARDHEIGHT; ++y)
    {
        for (auto x = 0; x < WIDTH; ++x)
        {
            SDL_Rect rect; // creating SDL rectangle object

            // Setting up the rectangle width, height and starting position(x and y)
            rect.x = x * DIM; 
            rect.y = y * DIM;
            rect.w = DIM;
            rect.h = DIM;

            // value of the board at x and y position
            BoardValue value = data->board[y][x];

            // if there boolian value is true, render(draw) rectangle and fill the rectangle to form shape
            if (value.isFilled)
            {
                SDL_SetRenderDrawColor(renderer, value.color.r, value.color.g, value.color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
            else // otherwise render an empty game play field with outer boxs
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