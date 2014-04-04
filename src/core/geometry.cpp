
#include "geometry.h"

///////////////////////////////////////////////////////////////////////////////
/// class Shape
///////////////////////////////////////////////////////////////////////////////
float Shape::get_height() const
{
    return -1;
}

float Shape::get_width() const
{
    return -1;
}

float Shape::get_min_x() const
{
    return 0;
}

float Shape::get_min_y() const
{
    return 0;
}

Shape::operator bool() const
{
    return (get_width() != -1 && get_height() != -1);
}

Shape* Shape::clone()
{
    return new Shape(*this);
}

///////////////////////////////////////////////////////////////////////////////
/// class Polygon
///////////////////////////////////////////////////////////////////////////////
Polygon::Polygon(vector<float> points) :
    points(points)
{
}

Polygon::Polygon()
{
    points = vector<float>();
}

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

float Rectangle::get_min_x() const
{
    return posx;
}

float Rectangle::get_min_y() const
{
    return posy;
}

Shape* Rectangle::clone()
{
    return new Rectangle(*this);
}
///////////////////////////////////////////////////////////////////////////////
