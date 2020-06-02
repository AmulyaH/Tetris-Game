
#include "shape.hpp"



Shape::Shape(SDL_Color color, bool matrix[4][4], double x, double y, int size): 
                            _color{color}, _matrix{matrix}, _x{x},  _y{y},  _size{size}{}


bool Shape::isBlock(Shape currShape, int x , int y)
{
    return currShape._matrix[x][y];
}