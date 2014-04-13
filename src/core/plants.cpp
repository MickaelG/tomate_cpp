
#include "plants.h"

#include "crops.h"

///////////////////////////////////////////////////////////////////////////////
// class Plants
///////////////////////////////////////////////////////////////////////////////

Plants::Plants(const Crops& crops) : crops(crops)
{
}

Plant& Plants::index(int data_index)
{
    if (data_index >= 0)
    {
        Plants::iterator it = this->begin();
        advance(it, data_index);
        return *it;
    }
    else
    {
        throw invalid_argument("Index must not be negative");
    }
}

const Plant& Plants::index(int data_index) const
{
    if (data_index >= 0)
    {
        Plants::const_iterator it = this->begin();
        advance(it, data_index);
        return *it;
    }
    else
    {
        return NullPlant;
    }
}

Plant& Plants::add_plant(string key, string name)
{
    if (key == "")
    {
        key = to_string(size());
    }
    if (get_pplant(key))
    {
        throw invalid_argument("Plant already exists in plot list");
    }
    push_back(Plant(key, name));
    return back();
}

Plant& Plants::get_plant(string key)
{
    Plant* pplant = get_pplant(key);
    if (pplant)
    {
        return *pplant;
    }
    else
    {
        throw invalid_argument("Plant with required key does not exist");
    }
}

Plant* Plants::get_pplant(string key)
{
    for (Plants::iterator it=this->begin(); it != this->end(); ++it)
    {
        if (it->get_key() == key)
        {
            return &(*it);
        }
    }
    return NULL;
}

int Plants::delete_plant(string key)
{
    Plant& del_plant = get_plant(key);
    return delete_plant(del_plant);
}

int Plants::delete_plant(int del_index)
{
    Plant& del_plant = index(del_index);
    return delete_plant(del_plant);
}

int Plants::delete_plant(Plant& plant)
{
    if ( ! crops.is_used_plant(plant) )
    {
        remove(plant);
        return 0;
    }
    else
    {
        return -1;
    }
}

bool Plants::is_used(int in_index) const
{
    return is_used(index(in_index));
}

bool Plants::is_used(const Plant& plant) const
{
    return crops.is_used_plant(plant);
}
///////////////////////////////////////////////////////////////////////////////

