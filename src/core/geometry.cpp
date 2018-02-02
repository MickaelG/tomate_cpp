
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
// class Rectangle
///////////////////////////////////////////////////////////////////////////////
Rectangle::Rectangle(int posx, int posy, int width, int height) :
    _rect(posx, posy, width, height)
{
}

Rectangle::Rectangle(int width, int height) :
    _rect(0, 0, width, height)
{
}

Rectangle::Rectangle(QRectF rect) :
    _rect(std::move(rect))
{
}

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(const Rectangle &other) :
    _rect(other._rect)
{
}

float Rectangle::get_width() const
{
    return _rect.width();
}

float Rectangle::get_height() const
{
    return _rect.height();
}

void Rectangle::set_width(float width)
{
    _rect.setWidth(width);
}

void Rectangle::set_height(float height)
{
    _rect.setHeight(height);
}

float Rectangle::get_min_x() const
{
    return _rect.left();
}

float Rectangle::get_min_y() const
{
    return _rect.top();
}

float Rectangle::get_max_x() const
{
    return _rect.right();
}

float Rectangle::get_max_y() const
{
    return _rect.bottom();
}

float Rectangle::get_area() const
{
    return _rect.width() * _rect.height();
}

const QRectF& Rectangle::rect() const
{
    return _rect;
}

void Rectangle::fit_in_other(const Shape& other)
{
    if (_rect.bottom() <= other.rect().top()) {
        _rect.setHeight(10);
        _rect.moveBottom(other.rect().top() + 10);
    } else if (_rect.top() >= other.rect().bottom()) {
        _rect.setHeight(10);
        _rect.moveTop(other.rect().bottom() - 10);
    }

    if (_rect.left() >= other.rect().right()) {
        _rect.setWidth(10);
        _rect.moveLeft(other.rect().right() - 10);
    } else if (_rect.right() <= other.rect().left()) {
        _rect.setWidth(10);
        _rect.moveRight(other.rect().left() + 10);
    }

    _rect &= other.rect();
}

void Rectangle::translate(float x_shift, float y_shift)
{
    _rect.translate(x_shift, y_shift);
}

bool Rectangle::is_inside(const Shape& other) const
{
    return other.rect().contains(_rect);
}

bool Rectangle::overlaps(const Shape& other) const
{
    return _rect.intersects(other.rect());
}

Rectangle Rectangle::intersection(const Rectangle& other) const
{
    return Rectangle(_rect & other.rect());
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

Rectangle* Rectangle::clone()
{
    return new Rectangle(*this);
}

string Rectangle::str() const
{
    return "(" + to_string(get_min_x()) + ", " + to_string(get_min_y()) + ", " +
           to_string(get_width()) + ", " + to_string(get_height()) + ")";
}

///////////////////////////////////////////////////////////////////////////////
