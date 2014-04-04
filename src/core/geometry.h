
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
/// class Shape : abstract class for all others
///////////////////////////////////////////////////////////////////////////////
class Shape
{
public:
    virtual float get_width() const;
    virtual float get_height() const;
    virtual float get_min_x() const;
    virtual float get_min_y() const;
    explicit operator bool() const;
    virtual Shape* clone();
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
    float get_width() const;
    float get_height() const;
    float get_min_x() const;
    float get_min_y() const;
    Shape* clone();
};

#endif //GEOMETRY_H
