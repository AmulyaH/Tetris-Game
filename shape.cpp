
#include "shape.hpp"
#include <string>
#include<iostream>

using namespace std;

Shape::Shape(SDL_Color color, bool matrix[4][4], double x, double y, int size): 
                            _color{color}, _matrix{matrix}, _x{x},  _y{y},  _size{size}{}


bool Shape::isBlock(Shape currShape, int x , int y)
{
    return currShape._matrix[x][y];
}

void moveShape(Shape *currShape, string position)
{
    if (position == "down")
        currShape->_y++;

    if (position == "left")
        currShape->_x--;

    if (position == "right")
        currShape->_x++;
}

 void rotateShape(Shape *currShape)
 {
    Shape temp = *currShape;
    for (int i = 0; i < temp._size; i++)
    {
        for (int j = 0; j < temp._size; j++)
        {
            currShape->_matrix[i][j] = temp._matrix[j][i];
        }
    }

    temp = *currShape;
    for (int i = 0; i < temp._size; i++)
    {
        for (int j = 0; j < temp._size / 2; j++)
        {
            bool t = temp._matrix[i][j];
            currShape->_matrix[i][j] = temp._matrix[i][temp._size - j - 1];
            currShape->_matrix[i][temp._size - j - 1] = t;
        }
    }
 }

 void renderShape(Shape currShape, SDL_Renderer *renderer)
 {
     SDL_Rect rect;
     for (int i = 0; i < currShape._size; i++)
    {
        for (int j = 0; j < currShape._size; j++)
        {
            if (currShape._matrix[i][j])
            {
                rect.x = (currShape._x + i) * DIM;
                rect.y = (currShape._y + j) * DIM;
                rect.w = DIM;
                rect.h = DIM;
                SDL_SetRenderDrawColor(renderer, currShape._color.r, currShape._color.g, currShape._color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
 }

 void renderNextShape(Shape nextShape, SDL_Renderer *renderer)
 {
     SDL_Rect rect;
     for (int i = 0; i < nextShape._size; i++)
    {
        for (int j = 0; j < nextShape._size; j++)
        {
            if (nextShape._matrix[i][j])
            {
                rect.x = (13 + i) * DIM;
                rect.y = (12 + j) * DIM;
                rect.w = DIM;
                rect.h = DIM;

                SDL_SetRenderDrawColor(renderer, nextShape._color.r, nextShape._color.g, nextShape._color.b, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
 }

 void saveShape(Gamedata *data, Shape current_shape, SDL_Renderer *renderer, int *score)
 {
     for (auto x = 0; x < current_shape._size; ++x)
    {
        for (auto y = 0; y < current_shape._size; ++y)
        {
            if (current_shape.isBlock(current_shape, x, y))
            {
                int wx = current_shape._x + x;
                int wy = current_shape._y + y;
                data->board[wy][wx].isFilled = true;
                data->board[wy][wx].color = current_shape._color;
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