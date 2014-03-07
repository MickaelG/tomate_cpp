
#include "plot.h"
#include "plots.h"

///////////////////////////////////////////////////////////////////////////////
// class Plot
///////////////////////////////////////////////////////////////////////////////
Plot NullPlot;

Plot::Plot()
{
}

Plot::Plot(string key, string name, string descr, Rectangle rect) :
    KeyName(key, name, descr), geometry(rect)
{
}

Plot::Plot(string key, string name, string descr, float width, float height, float posx, float posy) :
    KeyName(key, name, descr)
{
    geometry = Rectangle(width, height, posx, posy);
}

Rectangle Plot::get_rect()
{
    return geometry;
}

void Plot::set_rect(Rectangle rect)
{
    geometry = rect;
}

const list<Plot>& Plot::get_subplots() const
{
    return subplots;
}

list<Plot>& Plot::get_subplots()
{
    return subplots;
}

void Plot::add_subplot(float width, float height, float posx, float posy)
{
    int key_num = subplots.size() + 1;
    string subd_key = key + "-" + to_string(key_num);
    subplots.push_back(Plot(subd_key, "", "", width, height, posx, posy));
}

void Plot::create_subplots(int nb_hor, int nb_vert)
{
   float total_height = get_rect().get_height();
   float total_width = get_rect().get_width();
   float width = total_width / nb_hor;
   float height = total_height / nb_vert;

   for (int iy=0; iy < nb_vert; iy++)
   {
       for (int ix=0; ix < nb_hor; ix++)
       {
           add_subplot(width, height, ix*width, iy*height);
       }
   }
}

Plot& Plot::get_subplot(string key)
{
    for (Plots::iterator it=subplots.begin(); it != subplots.end(); ++it)
    {
        if (it->get_key() == key)
        {
            return *it;
        }

    }
    return NullPlot;
}
///////////////////////////////////////////////////////////////////////////////
