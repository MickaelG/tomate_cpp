
#include "plots.h"


#include "plot.h"
#include "crops.h"

///////////////////////////////////////////////////////////////////////////////
// class Plots
///////////////////////////////////////////////////////////////////////////////

Plots::Plots(const Crops& crops) : crops(crops)
{
}

const Plot& Plots::index(int data_index) const
{
    if (data_index >= 0)
    {
        Plots::const_iterator it = this->begin();
        advance(it, data_index);
        return *it;
    }
    else
    {
        return NullPlot;
    }
}

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
    if ( ! crops.is_used_plot(plot) )
    {
        remove(plot);
    }
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

bool Plots::is_used(int in_index) const
{
    return is_used(index(in_index));
}

bool Plots::is_used(const Plot& plot) const
{
    return crops.is_used_plot(plot);
}
///////////////////////////////////////////////////////////////////////////////
