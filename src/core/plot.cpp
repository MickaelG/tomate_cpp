
#include "plot.h"
#include "plots.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// class Plot
///////////////////////////////////////////////////////////////////////////////

Plot::Plot() :
    shape(new Rectangle(0, 0, -1, -1))
{
}

Plot::Plot(string name, string descr, unique_ptr< Shape > shape) :
    name(name), descr(descr), shape(std::move(shape))
{
}

Plot::Plot(string name, string descr, float width, float height, float posx, float posy) :
    name(name), descr(descr), shape(new Rectangle(posx, posy, width, height))
{
}

Plot::Plot(const Plot& plot) :
    name(plot.name), descr(plot.descr)
{
    if(plot.shape != nullptr)
    {
        shape.reset(plot.shape->clone());
    }
}

Plot::~Plot()
{
}

Shape& Plot::get_shape()
{
    return *shape;
}

const Shape& Plot::get_shape() const
{
    return *shape;
}

void Plot::set_shape(unique_ptr< Shape > in_shape)
{
    shape = std::move(in_shape);
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
