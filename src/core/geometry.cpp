
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

void Shape::fit_in_plot(const Shape* parent_shape)
{
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

void Rectangle::fit_in_plot(const Shape* parent_shape)
{
    if (!parent_shape || parent_shape->get_width() < 0)
    {
        return;
    }

    if (posx > parent_shape->get_width())
    {
        posx = parent_shape->get_width() - 10;
        width = 10;
    }
    else if (posx + width  > parent_shape->get_width())
    {
        width = parent_shape->get_width() - posx;
    }

    if (posy > parent_shape->get_height())
    {
        posy = parent_shape->get_height() - 10;
        height = 10;
    }
    else if (posy + height  > parent_shape->get_height())
    {
        height = parent_shape->get_height() - posy;
    }
}

Shape* Rectangle::clone()
{
    Rectangle* shape = new Rectangle(*this);
    return shape;
}
///////////////////////////////////////////////////////////////////////////////
