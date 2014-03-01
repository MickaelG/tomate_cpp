
#include "geometry.h"

///////////////////////////////////////////////////////////////////////////////
// class Rectangle
///////////////////////////////////////////////////////////////////////////////
Rectangle::Rectangle(int width, int height, int posx, int posy) :
    width(width), height(height), posx(posx), posy(posy) {}


Rectangle::Rectangle()
{
    width = 0;
    height = 0;
    posx = 0;
    posy = 0;
}

float Rectangle::get_width() const
{
    return width;
}

float Rectangle::get_height() const
{
    return height;
}

float Rectangle::get_x() const
{
    return posx;
}

float Rectangle::get_y() const
{
    return posy;
}

Rectangle::operator bool() const
{
    return !(posx == -1);
}
///////////////////////////////////////////////////////////////////////////////
