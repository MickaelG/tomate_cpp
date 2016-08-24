
#include "plot.h"
#include "plots.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// class Plot
///////////////////////////////////////////////////////////////////////////////

Plot::Plot() : shape(NULL)
{
}

Plot::Plot(string name, string descr, Shape* shape) :
    name(name), descr(descr), shape(shape)
{
}

Plot::Plot(string name, string descr, float width, float height, float posx, float posy) :
    name(name), descr(descr), shape(NULL)
{
    shape = new Rectangle(posx, posy, width, height);
}

Plot::Plot(const Plot& plot) :
    name(plot.name), descr(plot.descr), shape(NULL)
{
    if(plot.shape)
    {
        shape = plot.shape->clone();
    }
}

Plot::~Plot()
{
    if (shape)
    {
        delete shape;
        shape = NULL;
    }
}

Shape* Plot::get_shape()
{
    if (!shape)
    {
        shape = new Rectangle(0, 0, -1, -1);
    }
    return shape;
}

const Shape* Plot::get_shape() const
{
    return shape;
}

void Plot::set_shape(Shape* in_shape)
{
    if (shape)
    {
        delete shape;
        shape = NULL;
    }
    shape = in_shape;
}

void Plot::set_note(string new_note)
{
    descr = std::move(new_note);
}

const string &Plot::get_note() const
{
   return descr;
}

void Plot::set_name(string new_name)
{
    name = std::move(new_name);
}

const string &Plot::get_name() const
{
   return name;
}

bool operator==(const Plot& lhs, const Plot& rhs)
{
    return &lhs == &rhs;
}


///////////////////////////////////////////////////////////////////////////////
