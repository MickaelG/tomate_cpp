
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <set>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
/// class Shape : abstract class for all others
///////////////////////////////////////////////////////////////////////////////
class Shape
{
public:
    virtual float get_width() const = 0;
    virtual float get_height() const = 0;
    virtual float get_min_x() const = 0;
    virtual float get_min_y() const = 0;
    virtual float get_max_x() const = 0;
    virtual float get_max_y() const = 0;
    virtual float get_area() const = 0;
    virtual void fit_in_plot(const Shape* parent_shape) = 0;
    explicit operator bool() const ;
    virtual Shape* clone() = 0;
};

class Polygon: public Shape
{
private:
    vector<float> points;
public:
    Polygon();
    Polygon(vector<float> points);
};

class Rectangle: public Shape
{
private:
    int width, height;
    int posx, posy;
public:
    Rectangle();
    Rectangle(int width, int height, int posx, int posy);
    Rectangle(Shape& shape);
    float get_width() const;
    float get_height() const;
    float get_min_x() const;
    float get_min_y() const;
    float get_max_x() const;
    float get_max_y() const;
    float get_area() const;
    virtual void fit_in_plot(const Shape* parent_shape);
    bool is_inside(const Rectangle& other) const;
    bool overlaps(const Rectangle& other) const;
    Rectangle intersection(const Rectangle& other) const;
    Shape* clone();
};

set<Rectangle> get_split_partitions(const Rectangle& first, const Rectangle& other);


inline bool operator==(const Rectangle& lhs, const Rectangle& rhs)
{
    return (lhs.get_min_x() == rhs.get_min_x() && lhs.get_min_y() == rhs.get_min_y() &&
            lhs.get_width() == rhs.get_width() && lhs.get_height() == rhs.get_height());
}
inline bool operator!=(const Rectangle& lhs, const Rectangle& rhs) {return !operator==(lhs,rhs);}
inline bool operator< (const Rectangle& lhs, const Rectangle& rhs)
{
    if (lhs.get_min_x() < rhs.get_min_x()) return true;
    if (lhs.get_min_x() == rhs.get_min_x() && lhs.get_min_y() < rhs.get_min_y()) return true;
    if (lhs.get_min_x() == rhs.get_min_x() && lhs.get_min_y() == rhs.get_min_y() && lhs.get_width() < rhs.get_width()) return true;
    if (lhs.get_min_x() == rhs.get_min_x() && lhs.get_min_y() == rhs.get_min_y() && lhs.get_width() == rhs.get_width() && lhs.get_height() < rhs.get_height()) return true;
    return false;
}
inline bool operator> (const Rectangle& lhs, const Rectangle& rhs) {return  operator< (rhs,lhs);}
inline bool operator<=(const Rectangle& lhs, const Rectangle& rhs) {return !operator> (lhs,rhs);}
inline bool operator>=(const Rectangle& lhs, const Rectangle& rhs) {return !operator< (lhs,rhs);}

#endif //GEOMETRY_H
