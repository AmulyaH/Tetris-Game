#pragma once
#include "SDL2/SDL.h"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "board.hpp"


class Shape
{
    public:
        SDL_Color _color;
        bool _matrix[4][4];
        double _x, _y;
        int _size;

    public:
        Shape();
        Shape(SDL_Color _col, bool _mat[4][4], double x, double y, int size);
        bool isBlock(Shape shape, int x, int y);
        void moveShape();
};

bool shape1[4][4] = {0, 0, 1, 0,          // L
                     1, 1, 1, 0,
                     0, 0, 0, 0,
                     0, 0, 0, 0};

bool shape2[4][4] = {1, 1, 0, 0,          // Z
                     0, 1, 1, 0,
                     0, 0, 0, 0,
                     0, 0, 0, 0};

bool shape3[4][4] = {1, 1, 1, 1,          // I
                     0, 0, 0, 0,
                     0, 0, 0, 0,
                     0, 0, 0, 0};

bool shape4[4][4] = {1, 0, 0, 0,          // J
                     1, 1, 1, 0,
                     0, 0, 0, 0,
                     0, 0, 0, 0};

bool shape5[4][4] = {1, 1, 0, 0,          // 0
                     1, 1, 0, 0,
                     0, 0, 0, 0,
                     0, 0, 0, 0};

bool shape6[4][4] = {0, 1, 1, 0,          // S
                     1, 1, 0, 0,
                     0, 0, 0, 0,
                     0, 0, 0, 0};

bool shape7[4][4] =  {0, 1, 0, 0,         // T
                      1, 1, 1, 0,
                      0, 0, 0, 0,
                      0, 0, 0, 0};


Shape _shapes[7] = { Shape(SDL_Color{255, 0, 0},shape1,0,0,3) ,
                     Shape(SDL_Color{0, 255, 0},shape2,0,0,3) ,
                     Shape(SDL_Color{0, 0, 255},shape3,0,0,4) ,
                     Shape(SDL_Color{255, 255, 0},shape4,0,0,3) ,
                     Shape(SDL_Color{0, 255, 255},shape5,0,0,3) ,
                     Shape(SDL_Color{255, 0, 255},shape6,0,0,3) ,
                     Shape(SDL_Color{255, 255, 255},shape7,0,0,3) }

    
    
    
    
    
    

 
   


