
#include "plant.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// class Plant
///////////////////////////////////////////////////////////////////////////////

Plant::Plant()
{
}

Plant::Plant(string key, string name, string note, string color) :
    KeyName(key, name, note), color(color)
{
}

void Plant::add_var(string key, string name, string note)
{
    varlist.add(key, name, note);
}

string Plant::get_color_str() const
{
    return color;
}

void Plant::set_color_str(string color)
{
    this->color = color;
}

Vars& Plant::get_vars()
{
    return varlist;
}

const Vars& Plant::get_vars() const
{
    return varlist;
}
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// class Var
///////////////////////////////////////////////////////////////////////////////
Var::Var(string key, string name, string note) :
    KeyName(key, name, note)
{
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// class Vars
///////////////////////////////////////////////////////////////////////////////
Vars::Vars()
{
}

void Vars::add(string key, string name, string note)
{
    if (key == "")
    {
        key = to_string(_vvars.size());
    }
    //TODO : check that key is unique
    _vvars.push_back(unique_ptr<Var>(new Var(key, name, note)));
}

my_iterator<Var> Vars::begin()
{
    return my_iterator<Var>(_vvars.begin());
}

my_iterator<Var> Vars::end()
{
    return my_iterator<Var>(_vvars.end());
}

my_const_iterator<Var> Vars::begin() const
{
    return my_const_iterator<Var>(_vvars.begin());
}

my_const_iterator<Var> Vars::end() const
{
    return my_const_iterator<Var>(_vvars.end());
}

int Vars::size() const
{
    return _vvars.size();
}
///////////////////////////////////////////////////////////////////////////////
