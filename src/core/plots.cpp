
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

Plot& Plots::add(string key, const string& name, const string& descr, float width, float height, float posx, float posy)
{
    if (key == "")
    {
        key = to_string(_vplots.size());
    }
    _vplots.push_back(unique_ptr<Plot>(new Plot(key, name, descr, width, height, posx, posy)));
    return *_vplots.back();
}

Plot* Plots::find(const string& key)
{
    for (auto& plot_up: _vplots) {
        if (plot_up->get_key() == key) {
            return plot_up.get();
        }
    }
    return nullptr;
}

void Plots::remove(int index)
{
    if (!crops.is_used_plot(*_vplots[index])) {
        _vplots.erase(_vplots.begin() + index);
    }
}

void Plots::remove(const string& key)
{
    //TODO: use std::find
    auto plot_it = _vplots.begin();
    for ( ; plot_it != _vplots.end(); ++plot_it) {
        if ( (*(*plot_it)).get_key() == key ) {
            break;
        }
    }

    if (plot_it != _vplots.end()) {
        if ( !crops.is_used_plot(*(*plot_it)) ) {
            _vplots.erase(plot_it);
        }
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

///////////////////////////////////////////////////////////////////////////////
