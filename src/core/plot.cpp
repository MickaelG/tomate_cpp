
#include "plot.h"
#include "plots.h"

///////////////////////////////////////////////////////////////////////////////
// class Plot
///////////////////////////////////////////////////////////////////////////////
const Plot NullPlot;

Plot::Plot() : shape(NULL)
{
}

Plot::Plot(string key, string name, string descr, Shape* shape) :
    KeyName(key, name, descr), shape(shape)
{
}

Plot::Plot(string key, string name, string descr, float width, float height, float posx, float posy) :
    KeyName(key, name, descr), shape(NULL)
{
    shape = new Rectangle(posx, posy, width, height);
}

Plot::Plot(const Plot& plot) :
    KeyName(plot.key, plot.name, plot.note), shape(NULL)
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

///////////////////////////////////////////////////////////////////////////////
