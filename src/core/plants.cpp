
#include "plants.h"

#include "crops.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
// class Plants
///////////////////////////////////////////////////////////////////////////////

Plants::Plants(const Crops& crops) : crops(crops)
{
}

Plant& Plants::add(string key, const string& name, const string& note)
{
    if (key == "")
    {
        key = to_string(_vplants.size());
    }
    _vplants.push_back(unique_ptr<Plant>(new Plant(key, name, note)));
    return *_vplants.back();
}

Plant* Plants::find(const std::string& key)
{
    for (auto& plant_up: _vplants) {
        if (plant_up->get_key() == key) {
            return plant_up.get();
        }
    }
    return nullptr;
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

my_iterator<Plant> Plants::begin()
{
    return my_iterator<Plant>(_vplants.begin());
}

my_iterator<Plant> Plants::end()
{
    return my_iterator<Plant>(_vplants.end());
}

my_const_iterator<Plant> Plants::begin() const
{
    return my_const_iterator<Plant>(_vplants.begin());
}

my_const_iterator<Plant> Plants::end() const
{
    return my_const_iterator<Plant>(_vplants.end());
}

int Plants::size() const
{
    return _vplants.size();
}

const Plant& Plants::operator[](int index) const
{
    return *_vplants[index];
}

Plant& Plants::operator[](int index)
{
    return *_vplants[index];
}

///////////////////////////////////////////////////////////////////////////////

