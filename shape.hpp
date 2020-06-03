#pragma once
#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include <string>
#include "board.hpp"

using namespace std;

/**
 * Shape class which initialize the shapes, move shapes and save shapes in board
 */ 
class Shape
{
     public:
        SDL_Color _color; // to set the color of the shape
        bool _matrix[4][4];
        double _offsetX, _offsetY;
        int _size;


    public:
        Shape() = default;
        Shape(SDL_Color _col, bool _mat[4][4], double x, double y, int size);

        // to check if the boack is filled for the given row and col position in a shape matrix
        bool isBlock(Shape shape, int row, int col);

        // to move the shapes to left, right and down based on the key press on keyboard
        void moveShape(Shape *shape, string position);

        // rotate the shape on press on pgup key
        void rotateShape(Shape *shape);

        // render shape and dispaly on board
        void renderShape(Shape currShape, SDL_Renderer *renderer);

        // render the next droping shape and display on board
        void renderNextShape(Shape nextShape, SDL_Renderer *renderer);

        // when shape gets collied with board or with othershape, save the same in that location of the board 
        void saveShape(Gamedata *data, Shape currShape, SDL_Renderer *renderer, int &score); 
        
};
    
    

 
   


