
#include "plot.h"
#include "plots.h"

///////////////////////////////////////////////////////////////////////////////
// class Plot
///////////////////////////////////////////////////////////////////////////////
Plot NullPlot;

Plot::Plot() : shape(NULL)
{
}

Plot::Plot(string key, string name, string descr, Shape* shape) :
    KeyName(key, name, descr), shape(shape)
{
}

Plot::Plot(string key, string name, string descr, float width, float height, float posx, float posy) :
    KeyName(key, name, descr)
{
    shape = new Rectangle(width, height, posx, posy);
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
