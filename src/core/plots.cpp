
#include "plots.h"


#include "plot.h"
#include "crops.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// class Plots
///////////////////////////////////////////////////////////////////////////////

Plots::Plots(const Crops& crops) : crops(crops)
{
}

Plot& Plots::add(const string& name, const string& descr, float width, float height, float posx, float posy)
{
    _vplots.push_back(unique_ptr<Plot>(new Plot(name, descr, width, height, posx, posy)));
    return *_vplots.back();
}

void Plots::remove(int index)
{
    if (!crops.is_used_plot(*_vplots[index])) {
        _vplots.erase(_vplots.begin() + index);
    }
}

bool Plots::is_used(const Plot& plot) const
{
    return crops.is_used_plot(plot);
}

my_iterator<Plot> Plots::begin()
{
    return my_iterator<Plot>(_vplots.begin());
}

my_iterator<Plot> Plots::end()
{
    return my_iterator<Plot>(_vplots.end());
}

my_const_iterator<Plot> Plots::begin() const
{
    return my_const_iterator<Plot>(_vplots.begin());
}

my_const_iterator<Plot> Plots::end() const
{
    return my_const_iterator<Plot>(_vplots.end());
}

int Plots::size() const
{
    return _vplots.size();
}

const Plot& Plots::operator[](int index) const
{
    return *_vplots[index];
}

Plot& Plots::operator[](int index)
{
    return *_vplots[index];
}

Plot* Plots::get_for_pos(const Shape& shape)
{
    for (const auto& plot: _vplots) {
        if (plot->get_shape().overlaps(shape)) {
            return &(*plot);
        }
    }
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
