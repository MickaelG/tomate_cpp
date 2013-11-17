#include "dataset.h"


Rectangle::Rectangle(int width, int height, int posx, int posy) :
    width(width), height(height), posx(posx), posy(posy) {}


Rectangle::Rectangle()
{
    width = 0;
    height = 0;
    posx = 0;
    posy = 0;
}


Var::Var(string key, string name, string note) :
    KeyName(key, name), note(note) {}

Plot NullPlot;
Plot::Plot(string key, string name, string descr, Rectangle rect) :
    KeyName(key, name), geometry(rect), descr(descr) {}

Plot::Plot(string key, string name, string descr, float width, float height, float posx, float posy) :
    KeyName(key, name), descr(descr)
{
    geometry = Rectangle(width, height, posx, posy);
}

void Plot::add_subplot(float width, float height, float posx, float posy)
{
    int key_num = subplots.size() + 1;
    string subd_key = key + "-" + to_string(key_num);
    subplots.push_back(Plot(subd_key, "", "", width, height, posx, posy));
}

string Crop::str_descr() const
{
    return plant.get_name();
}

bg::date Crop::get_date(string which) const
{
    if (which == "planned_start")
    {
        return planned_start_date;
    }
    else if (which == "planned_end")
    {
        return planned_end_date;
    }
    else if (which == "end")
    {
        return end_date;
    }
    else
    {
        return start_date;
    }
}

void Dataset::add_crop(Crop crop)
{
    this->crops.push_back(crop);
}

void Dataset::add_plot(string key, string name, string descr, float width, float height, float posx, float posy)
{
    this->plots.push_back(Plot(key, name, descr, width, height, posx, posy));
}
void Dataset::add_plot(Plot plot)
{
    this->plots.push_back(plot);
}

Plant& Dataset::get_plant(string key)
{
    for (int i = 0; i < plants.size(); i++)
    {
        if (plants[i].get_key() == key)
        {
            return plants[i];
        }
    }
    return NullPlant;
}

Plant NullPlant;
Plant::Plant(string key, string name, string note, string color) :
    KeyName(key, name), note(note), color(color) {};

//TODO: search hierarchically without running through all subplots
Plot& Dataset::get_plot(string key)
{
    for (int i_plot = 0; i_plot < plots.size(); i_plot++)
    {
        if (plots[i_plot].get_key() == key)
        {
            return plots[i_plot];
        }
        if (plots[i_plot].get_subplot(key))
        {
            return plots[i_plot].get_subplot(key);
        }
    }
    return NullPlot;
}

Plot& Plot::get_subplot(string key)
{
    for (int i_plot = 0; i_plot < subplots.size(); i_plot++)
    {
        if (subplots[i_plot].get_key() == key)
        {
            return subplots[i_plot];
        }

    }
    return NullPlot;
}
