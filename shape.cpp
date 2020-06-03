#include "shape.hpp"
#include <string>
#include <iostream>

using namespace std;


// construtor to initialize the shape with details of color, offset values, size
Shape::Shape(SDL_Color color, bool matrix[4][4], double x, double y, int size) : 
            _color{color}, _matrix{matrix}, _offsetX{x}, _offsetY{y}, _size{size}
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            _matrix[row][col] = matrix[row][col];
        }
    }
};

/**
 * to check if the boack is filled for the given row and col position in a shape matrix
 * @param currentShape current shape
 * @param row row number in shape matrix
 * @param col column number in shape matrix
 * @return bool value true/false
 */ 
bool Shape::isBlock(Shape currentShape, int row, int col)
{
    return currentShape._matrix[row][col];
}

/**
 * to move the shapes to left, right and down based on the key press on keyboard
 * @param currentShape current shape
 * @param direction the direction movement has to be done 
 */ 
void Shape::moveShape(Shape *currentShape, string direction)
{
    if (direction == "down")
        currentShape->_offsetY++;

    if (direction == "left")
        currentShape->_offsetX--;

    if (direction == "right")
        currentShape->_offsetX++;
}

/**
 * rotate the shape on press on page up key
 * @param currentShape the current shape which has to be rotated
 */ 
void Shape::rotateShape(Shape *currentShape)
{
    // doing the transpose of the shape
    Shape temp = *currentShape;
    for (int row = 0; row < temp._size; row++)
    {
        for (int col = 0; col < temp._size; col++)
        {
            currentShape->_matrix[row][col] = temp._matrix[col][row];
        }
    }

    // changing the columns
    temp = *currentShape;
    for (int row = 0; row < temp._size; row++)
    {
        for (int col = 0; col < temp._size / 2; col++)
        {
            bool isTrue = temp._matrix[row][col];
            currentShape->_matrix[row][col] = temp._matrix[row][temp._size - col - 1];
            currentShape->_matrix[row][temp._size - col - 1] = isTrue;
        }
    }
}

/**
 * render shape and dispaly on board
 * @param currentShape the shape which has to be rendered
 * @param renderer SDL renderer to render shape
 */
void Shape::renderShape(Shape currentShape, SDL_Renderer *renderer)
{
    SDL_Rect rect;
    for (int i = 0; i < currentShape._size; i++)
    {
        for (int j = 0; j < currentShape._size; j++)
        {
            if (currentShape._matrix[i][j])
            {
                rect.x = (currentShape._offsetX + i) * DIM;
                rect.y = (currentShape._offsetY + j) * DIM;
                rect.w = DIM;
                rect.h = DIM;
                SDL_SetRenderDrawColor(renderer, currentShape._color.r, currentShape._color.g, currentShape._color.b, currentShape._color.a);
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetRenderDrawColor(renderer, 255, 219, 219, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }
}

/**
 * render the next droping shape and display on board
 * @param nextShape the next shape which which will show up on board and has to be rendered
 * @param renderer SDL renderer to render shape
 */ 
void Shape::renderNextShape(Shape nextShape, SDL_Renderer *renderer)
{
    SDL_Rect rect;
    for (int i = 0; i < nextShape._size; i++)
    {
        for (int j = 0; j < nextShape._size; j++)
        {
            if (nextShape._matrix[i][j])
            {
                rect.x = (13 + i) * DIM;
                rect.y = (16 + j) * DIM;
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

/**
 * save the shape in the board and clear the line if all the columns in a row is filled 
 * @param data gamedata which has the info of which all blocks of the board is filled with shapes
 * @param currentShape the current shape which has to be saved on board
 * @param renderer SDL rederer to render and save the shape on board
 * @param score the score will be updated with + 10 when each row / line gets clear
 */ 
void Shape::saveShape(Gamedata *data, Shape currentShape, SDL_Renderer *renderer, int &score)
{
    for (auto x = 0; x < currentShape._size; ++x)
    {
        for (auto y = 0; y < currentShape._size; ++y)
        {
            if (currentShape.isBlock(currentShape, x, y))
            {
                int wx = currentShape._offsetX + x;
                int wy = currentShape._offsetY + y;
                data->board[wy][wx].isFilled = true;
                data->board[wy][wx].color = currentShape._color;
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
