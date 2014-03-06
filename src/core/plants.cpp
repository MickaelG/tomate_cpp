
#include "plants.h"

///////////////////////////////////////////////////////////////////////////////
// class Plant
///////////////////////////////////////////////////////////////////////////////
Plant NullPlant;
Plant::Plant(string key, string name, string note, string color) :
    KeyName(key, name, note), color(color) {};

void Plant::add_var(string key, string name, string note)
{
    if (key == "")
    {
        key = to_string(varlist.size());
    }
    //TODO : check that key is unique
    varlist.push_back(Var(key, name, note));
}

string Plant::get_color_str() const
{
    return color;
}

void Plant::set_color_str(string color)
{
    this->color = color;
}

Vars& Plant::get_vars() {
    return varlist;
}
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// class Plants
///////////////////////////////////////////////////////////////////////////////
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
        return NullPlant;
    }
}

Plant& Plants::add_plant(string key, string name)
{
    if (key == "")
    {
        key = to_string(size());
    }
    if (get_plant(key))
    {
        return NullPlant;
    }
    push_back(Plant(key, name));
    return back();
}

Plant& Plants::get_plant(string key)
{
    for (Plants::iterator it=this->begin(); it != this->end(); ++it)
    {
        if (it->get_key() == key)
        {
            return *it;
        }
    }
    return NullPlant;
}
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// class Var
///////////////////////////////////////////////////////////////////////////////
Var::Var(string key, string name, string note) :
    KeyName(key, name, note) {}
///////////////////////////////////////////////////////////////////////////////
