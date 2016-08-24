
#include "plants.h"

#include "crops.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// class Plants
///////////////////////////////////////////////////////////////////////////////

Plants::Plants(const Crops& crops) : crops(crops)
{
}

PlantSpecies& Plants::add(const string& name, const string& note)
{
    _vplants.push_back(unique_ptr<PlantSpecies>(new PlantSpecies(name, note)));
    return *_vplants.back();
}

void Plants::remove(int iplant)
{
    if (iplant >= _vplants.size())
    {
        return;
    }
    _vplants.erase(_vplants.begin() + iplant);
}

bool Plants::delete_plant(int iplant)
{
    if ( ! crops.is_used_plant(*_vplants[iplant]) )
    {
        remove(iplant);
        return true;
    }
    return false;
}

bool Plants::is_used(const Plant& plant) const
{
    return crops.is_used_plant(plant);
}

my_iterator<PlantSpecies> Plants::begin()
{
    return my_iterator<PlantSpecies>(_vplants.begin());
}

my_iterator<PlantSpecies> Plants::end()
{
    return my_iterator<PlantSpecies>(_vplants.end());
}

my_const_iterator<PlantSpecies> Plants::begin() const
{
    return my_const_iterator<PlantSpecies>(_vplants.begin());
}

my_const_iterator<PlantSpecies> Plants::end() const
{
    return my_const_iterator<PlantSpecies>(_vplants.end());
}

int Plants::size() const
{
    return _vplants.size();
}

const PlantSpecies& Plants::operator[](int index) const
{
    return *_vplants[index];
}

PlantSpecies& Plants::operator[](int index)
{
    return *_vplants[index];
}

///////////////////////////////////////////////////////////////////////////////

