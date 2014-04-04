#include "dataset.h"

#include "plot.h"
#include "plant.h"


///////////////////////////////////////////////////////////////////////////////
// class Dataset
///////////////////////////////////////////////////////////////////////////////

Dataset::Dataset() : plots(crops), plants(crops)
{
}

Crop& Dataset::add_crop(Crop crop)
{
    this->crops.push_back(crop);
    return crops.back();
}

Plant& Dataset::add_plant(Plant plant)
{
    plants.push_back(plant);
    return plants.back();
}

Plot& Dataset::add_plot(Plot& plot)
{
    return get_plots().add_plot(plot);
}

Plant& Dataset::get_plant(string key)
{
    return get_plants().get_plant(key);
}

void Dataset::set_filename(string in_filename)
{
    filename = in_filename;
}

const Plots& Dataset::get_plots() const
{
    return plots;
}

const Plants& Dataset::get_plants() const
{
    return plants;
}

const Crops& Dataset::get_crops() const {
    return crops;
}

Plots& Dataset::get_plots() {
    return plots;
}

Plants& Dataset::get_plants() {
    return plants;
}

Crops& Dataset::get_crops() {
    return crops;
}

//TODO: search hierarchically without running through all subplots
Plot& Dataset::get_plot(string key)
{
    for (Plots::iterator it=plots.begin(); it != plots.end(); ++it)
    {
        if (it->get_key() == key)
        {
            return *it;
        }
    }
    return NullPlot;
}
///////////////////////////////////////////////////////////////////////////////
