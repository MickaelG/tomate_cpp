#include "dataset.h"

#include "plot.h"
#include "plant.h"


///////////////////////////////////////////////////////////////////////////////
// class Dataset
///////////////////////////////////////////////////////////////////////////////

Dataset::Dataset() : _plots(_crops), _plants(_crops)
{
}

void Dataset::set_filename(string filename)
{
    _filename = filename;
}

const Plots& Dataset::get_plots() const
{
    return _plots;
}

const Plants& Dataset::get_plants() const
{
    return _plants;
}

const Crops& Dataset::get_crops() const {
    return _crops;
}

Plots& Dataset::get_plots() {
    return _plots;
}

Plants& Dataset::get_plants() {
    return _plants;
}

Crops& Dataset::get_crops() {
    return _crops;
}

///////////////////////////////////////////////////////////////////////////////
