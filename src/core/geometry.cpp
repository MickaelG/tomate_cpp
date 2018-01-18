
#include "geometry.h"
#include "log.h"
#include <algorithm>
#include <cassert>
#include <iostream>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
/// class Shape
///////////////////////////////////////////////////////////////////////////////
Shape::operator bool() const
{
    return (get_width() != -1 && get_height() != -1);
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
Rectangle::Rectangle(int posx, int posy, int width, int height) :
    width(width), height(height), posx(posx), posy(posy)
{
}

Rectangle::Rectangle(int width, int height) :
    width(width), height(height), posx(0), posy(0)
{
}

Rectangle::Rectangle()
{
    width = 0;
    height = 0;
    posx = 0;
    posy = 0;
}

Rectangle::Rectangle(const Shape &shape) :
    width(shape.get_width()), height(shape.get_height()),
    posx(shape.get_min_x()), posy(shape.get_min_y())
{
}

Rectangle::Rectangle(const Rectangle &shape) :
    width(shape.get_width()), height(shape.get_height()),
    posx(shape.get_min_x()), posy(shape.get_min_y())
{
}

float Rectangle::get_width() const
{
    return width;
}

float Rectangle::get_height() const
{
    return height;
}

void Rectangle::set_width(float in_width)
{
    width = in_width;
}

void Rectangle::set_height(float in_height)
{
    height = in_height;
}

float Rectangle::get_min_x() const
{
    return posx;
}

float Rectangle::get_min_y() const
{
    return posy;
}

float Rectangle::get_max_x() const
{
    return posx + width;
}

float Rectangle::get_max_y() const
{
    return posy + height;
}

float Rectangle::get_area() const
{
    return width * height;
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

void Rectangle::translate(float x_shift, float y_shift)
{
    posx += x_shift;
    posy += y_shift;
}

bool Rectangle::is_inside(const Shape& other) const
{
    //TODO: handle something else than rect
    return is_inside(dynamic_cast<const Rectangle&>(other));
}

bool Rectangle::is_inside(const Rectangle& other) const
{
    if (posx < other.posx || posy < other.posy) {
        return false;
    }
    if (posx + width > other.posx + other.width ||
        posy + height > other.posy + other.height) {
        return false;
    }
    return true;
}

bool Rectangle::overlaps(const Rectangle& other) const
{
    if (posx >= other.posx + other.width || posy >= other.posy + other.height) {
        return false;
    }
    if (other.posx >= posx + width || other.posy >= posy + height) {
        return false;
    }
    return true;
}

Rectangle Rectangle::intersection(const Rectangle& other) const
{
    if (!overlaps(other)) {
        return Rectangle(0, 0, -1, -1);
    }
    int x = max(posx, other.posx);
    int y = max(posy, other.posy);
    int x2 = min(get_max_x(), other.get_max_x());
    int y2 = min(get_max_y(), other.get_max_y());
    return Rectangle(x, y, x2-x, y2-y);
}

set<Rectangle> compute_non_overlapping_rects(const Rectangle& first, const Rectangle& other)
{
    set<Rectangle> result;
    if (!first.overlaps(other)) {
        Log::Warning("compute_non_overlapping_rects called on non-overlaping rects: " + first.str() + " / " + other.str());
        result.insert(first);
        result.insert(other);
        return result;
    }
    if (first.is_inside(other) || other.is_inside(first)) {
        vector< float > y_points;
        y_points.push_back(first.get_min_y());
        y_points.push_back(first.get_max_y());
        y_points.push_back(other.get_min_y());
        y_points.push_back(other.get_max_y());
        sort(y_points.begin(), y_points.end());
        y_points.erase( unique( y_points.begin(), y_points.end() ), y_points.end() );

        vector< float > x_points;
        x_points.push_back(first.get_min_x());
        x_points.push_back(first.get_max_x());
        x_points.push_back(other.get_min_x());
        x_points.push_back(other.get_max_x());
        sort(x_points.begin(), x_points.end());
        x_points.erase( unique( x_points.begin(), x_points.end() ), x_points.end() );

        for (int ix = 0; ix < x_points.size() - 1; ++ix) {
            for (int iy = 0; iy < y_points.size() - 1; ++iy) {
                result.insert(Rectangle(x_points[ix], y_points[iy],
                                        x_points[ix+1] - x_points[ix],
                                        y_points[iy+1] - y_points[iy]));
            }
        }
        return result;
    } else {
        Rectangle intersect = first.intersection(other);
        assert(intersect.is_inside(first) && intersect.is_inside(other));
        set<Rectangle> splitpart = compute_non_overlapping_rects(first, intersect);
        result.insert(splitpart.begin(), splitpart.end());
        splitpart = compute_non_overlapping_rects(other, intersect);
        result.insert(splitpart.begin(), splitpart.end());
        return result;
    }
}

std::ostream& operator<<(std::ostream& stream, const Rectangle& rect)
{
   stream << "Rectangle(";
   stream << rect.get_min_x() << ", ";
   stream << rect.get_min_y() << ", ";
   stream << rect.get_width() << ", ";
   stream << rect.get_height() << ")";
   return stream;
}

Shape* Rectangle::clone()
{
    Rectangle* shape = new Rectangle(*this);
    return shape;
}

string Rectangle::str() const
{
    return "(" + to_string(posx) + ", " + to_string(posy) + ", " +
           to_string(width) + ", " + to_string(height) + ")";
}

///////////////////////////////////////////////////////////////////////////////
