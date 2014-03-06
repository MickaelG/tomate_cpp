
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


///////////////////////////////////////////////////////////////////////////////
// class Plots
///////////////////////////////////////////////////////////////////////////////

Plot& Plots::index(int data_index)
{
    if (data_index >= 0)
    {
        Plots::iterator it = this->begin();
        advance(it, data_index);
        return *it;
    }
    else
    {
        return NullPlot;
    }
}

Plot& Plots::add_plot(string key, string name, string descr, float width, float height, float posx, float posy)
{
    if (key == "")
    {
        key = to_string(size());
    }
    return add_plot(Plot(key, name, descr, width, height, posx, posy));
}

Plot& Plots::add_plot(Plot plot)
{
    if (get_plot(plot.get_key()))
    {
        return NullPlot;
    }
    push_back(plot);
    return back();
}

void Plots::delete_plot(string key)
{
    Plot& del_plot = get_plot(key);
    delete_plot(del_plot);
}

void Plots::delete_plot(int del_index)
{
    Plot& del_plot = index(del_index);
    delete_plot(del_plot);
}

void Plots::delete_plot(Plot& plot)
{
    //TODO: check that plot is not used in crops
    remove(plot);
}

Plot& Plots::get_plot(string key)
{
    for (Plots::iterator it=this->begin(); it != this->end(); ++it)
    {
        if (it->get_key() == key)
        {
            return *it;
        }
    }
    return NullPlot;
}
///////////////////////////////////////////////////////////////////////////////
