
#include "plant.h"
using namespace std;


PlantSpecies::PlantSpecies(string name, string note, string color) :
    name(name), note(note), color(color)
{
}

PlantVariety& PlantSpecies::add_var(string name, string note)
{
    vars.push_back(unique_ptr<PlantVariety>(new PlantVariety(*this, name, note)));
    return *vars.back();
}

string PlantSpecies::get_color_str() const
{
    return color;
}

void PlantSpecies::set_color_str(string color)
{
    this->color = color;
}

const vector<unique_ptr<PlantVariety> >& PlantSpecies::get_vars() const
{
    return vars;
}

void PlantSpecies::set_note(string new_note)
{
    note = std::move(new_note);
}

const string &PlantSpecies::get_note() const
{
    return note;
}

const PlantSpecies &PlantSpecies::get_species() const
{
   return *this;
}

void PlantSpecies::set_name(string new_name)
{
    name = std::move(new_name);
}

string PlantSpecies::get_name() const
{
   return name;
}

bool operator==(const Plant& lhs, const Plant& rhs)
{
    return &lhs == &rhs;
}

PlantVariety::PlantVariety(PlantSpecies& species, string name, string note) :
    name(name),
    note(note),
    species(species)
{
}

void PlantVariety::set_note(string new_note)
{
    note = std::move(new_note);
}

const string &PlantVariety::get_note() const
{
   return note;
}

void PlantVariety::set_name(string new_name)
{
    name = std::move(new_name);
}

string PlantVariety::get_name() const
{
   return name;
}

string PlantVariety::get_color_str() const
{
    return species.get_color_str();
}

const PlantSpecies &PlantVariety::get_species() const
{
   return species;
}
