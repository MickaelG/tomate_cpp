
#include "crops.h"

#include "plot.h"
#include "plant.h"

#include <stdexcept>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// class Crop
///////////////////////////////////////////////////////////////////////////////
string Crop::get_repr() const
{
    return p_plant->get_name();
}

bg::date Crop::get_date(DateSel which) const
{
    switch (which) {
    case DateSel::START:
        return start_date;
        break;
    case DateSel::END:
        return end_date;
        break;
    case DateSel::PLANNED_END:
        return planned_end_date;
        break;
    case DateSel::PLANNED_START:
        return planned_start_date;
        break;
    default:
        throw "Erreur, unknown which";
        break;
    }
}

void Crop::set_date(DateSel which, bg::date date)
{
    switch (which) {
    case DateSel::START:
        start_date = date;
        break;
    case DateSel::END:
        end_date = date;
        break;
    case DateSel::PLANNED_END:
        planned_end_date = date;
        break;
    case DateSel::PLANNED_START:
        planned_start_date = date;
        break;
    default:
        throw "Erreur, unknown which";
        break;
    }
}

Crop::Crop() : p_plant(nullptr), p_plot(nullptr), shape(nullptr)
{
}

Crop::Crop(bg::date start_date, bg::date end_date,
     bg::date planned_start_date, bg::date planned_end_date,
     Plant* p_plant, const string& varkey,
     Plot* p_plot, const string& note, Rectangle rect) :
    start_date(start_date), end_date(end_date),
    planned_start_date(planned_start_date), planned_end_date(planned_end_date),
    varkey(varkey),
    p_plant(p_plant), p_plot(p_plot), note(note), shape(nullptr)
{
    if (rect.get_width() > 0) {
        set_shape(new Rectangle(rect));
    } else {
        set_default_shape();
    }
}

Crop::Crop(bg::date start_date, bg::date end_date,
     Plant* p_plant, const string& varkey,
     Plot* p_plot, const string& note) :
    start_date(start_date), end_date(end_date),
    varkey(varkey),
    p_plant(p_plant), p_plot(p_plot), note(note), shape(nullptr)
{
}

string Crop::description() const
{
    return get_plant().get_name() + " start:" + bg::to_simple_string(get_date(DateSel::START)) +
                                    " end:"   + bg::to_simple_string(get_date(DateSel::END)) +
                                    " planned_start:" + bg::to_simple_string(get_date(DateSel::PLANNED_START)) +
                                    " planned_end:"   + bg::to_simple_string(get_date(DateSel::PLANNED_END));
}

Plant* Crop::get_pplant()
{
    return p_plant;
}

Plant& Crop::get_plant()
{
    if (p_plant)
    {
        return *p_plant;
    } else {
        throw logic_error("Plant is not yet defined for crop");
    }
}

const Plant& Crop::get_plant() const
{
    if (p_plant) {
        return *p_plant;
    }
    throw logic_error("Plant not defined");
}

void Crop::set_plant(Plant& plant)
{
    p_plant = &plant;
}

Plot* Crop::get_pplot()
{
    return p_plot;
}

Plot& Crop::get_plot()
{
    if (p_plot) {
        return *p_plot;
    }
    throw logic_error("Plot is not yet defined for crop");
}

const Plot& Crop::get_plot() const
{
    if (p_plot) {
        return *p_plot;
    }
    throw logic_error("Plot is not yet defined for crop");
}

void Crop::set_plot(Plot& plot)
{
    p_plot = &plot;
}

void Crop::add_action(bg::date date, string note)
{
    actions.push_back(CropAction(date, note));
}

vector<CropAction>& Crop::get_actions()
{
    return actions;
}

const vector<CropAction>& Crop::get_actions() const
{
    return actions;
}

string Crop::get_varkey() const
{
    return varkey;
}

void Crop::set_varkey(string varkey)
{
    this->varkey = varkey;
}

Crop::operator bool() const
{
    if ((!p_plant) || (!p_plot))
    {
        return false;
    }
    return (*p_plant) && (*p_plot);
}


bool Crop::is_active_at_date(bg::date date) const
{
    bool result;
    if ((!start_date.is_not_a_date() && date >= start_date) &&
        (!get_virtual_end_date().is_not_a_date() && date <= get_virtual_end_date()))
    {
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
}

bool Crop::is_planned_at_date(bg::date date) const
{
    bool result;
    if ((!get_virtual_planned_start_date().is_not_a_date() && date > get_virtual_planned_start_date()) &&
        (!get_virtual_planned_end_date().is_not_a_date() && date < get_virtual_planned_end_date()))
    {
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
}

bool Crop::is_in_year_started_by(bg::date date) const
{
    if ((!get_virtual_end_date().is_not_a_date() && get_virtual_end_date() >= date) &&
        (!start_date.is_not_a_date() && start_date <= date + bg::date_duration(365)))
    {
        return true;
    }
    else if ((!planned_end_date.is_not_a_date() && planned_end_date >= date) &&
             (!planned_start_date.is_not_a_date() && planned_start_date <= date + bg::date_duration(365)))
    {
        return true;
    }
    return false;
}

bg::date Crop::get_virtual_end_date() const
{
    if (end_date.is_not_a_date() && (!start_date.is_not_a_date()))
    {
        return bg::day_clock::local_day();
    }
    else
    {
        return end_date;
    }
}

bg::date Crop::get_virtual_planned_start_date() const
{
    if ( planned_start_date.is_not_a_date() && (!planned_end_date.is_not_a_date()) )
    {
        return start_date;
    }
    else
    {
        return planned_start_date;
    }
}

bg::date Crop::get_virtual_planned_end_date() const
{
    if (planned_end_date.is_not_a_date() && (!planned_start_date.is_not_a_date()))
    {
        return planned_start_date + bg::days(14);
    }
    else
    {
        return planned_end_date;
    }
}

string Crop::get_note() const
{
    return note;
}

void Crop::set_note(string note)
{
    this->note = note;
}

Shape* Crop::get_shape()
{
    return shape;
}

const Shape* Crop::get_shape() const
{
    return shape;
}

void Crop::set_default_shape()
{
    if (shape == nullptr)
    {
        Shape* plot_shape = get_plot().get_shape();
        if (plot_shape)
        {
            shape = new Rectangle(0, 0, plot_shape->get_width(), plot_shape->get_height());
        }
        else
        {
            shape = new Rectangle(0, 0, -1, -1);
        }
    }
}

void Crop::set_shape(Shape* in_shape)
{
    if (shape)
    {
        delete shape;
        shape = nullptr;
    }
    shape = in_shape;
    if (p_plot)
    {
        shape->fit_in_plot(p_plot->get_shape());
    }
}

bool operator==(const Crop& elem1, const Crop& elem2)
{
    return (&elem1 == &elem2);
}

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// class Crops
///////////////////////////////////////////////////////////////////////////////
Crop& Crops::add(bg::date start_date, bg::date end_date,
                 bg::date planned_start_date, bg::date planned_end_date,
                 Plant *p_plant, const std::string& varkey,
                 Plot *p_plot, const std::string& note, Rectangle rect)
{
    _vcrops.push_back(unique_ptr<Crop>(new Crop(start_date, end_date, planned_start_date, planned_end_date,
                                                p_plant, varkey, p_plot, note, rect)));
    return *_vcrops.back();

}

Crop& Crops::add(const Crop& crop)
{
    _vcrops.push_back(unique_ptr<Crop>(new Crop(crop)));
    return *_vcrops.back();

}

vector<Crop*> Crops::find_crops(const Plot& plot, bg::date date)
{
    vector<Crop*> result;
    //for (int i_crop = 0; i_crop < this->size(); i_crop++)
    for (auto it = _vcrops.begin(); it != _vcrops.end(); ++it)
    {
        if ((*it)->get_plot() == plot)
        {
           if ((*it)->is_active_at_date(date) || (*it)->is_planned_at_date(date))
           {
               result.push_back(it->get());
           }
        }
    }
    return result;
}

bool Crops::is_used_plot(const Plot& plot) const
{
    string plot_key = plot.get_key();
    for (auto it = _vcrops.cbegin(); it != _vcrops.cend(); ++it)
    {
        string loop_key = (*it)->get_plot().get_key();
        //plot_key must be conntained in loop_key
        if (loop_key.find(plot_key) == 0)
        {
            return true;
        }
    }
    return false;
}

bool Crops::is_used_plant(const Plant& plant) const
{
    string plant_key = plant.get_key();
    for (auto it = _vcrops.cbegin(); it != _vcrops.cend(); ++it)
    {
        string loop_key = (*it)->get_plant().get_key();
        //plant_key must be conntained in loop_key
        if (loop_key.find(plant_key) == 0)
        {
            return true;
        }
    }
    return false;
}

void Crops::remove(const Crop& crop)
{
    auto crop_it = std::find_if(_vcrops.begin(), _vcrops.end(),
                                [&crop](const unique_ptr<Crop>& value) { return *value == crop; } );
    if (crop_it != _vcrops.end()) {
        _vcrops.erase(crop_it);
    }
}

my_iterator<Crop> Crops::begin()
{
    return my_iterator<Crop>(_vcrops.begin());
}

my_iterator<Crop> Crops::end()
{
    return my_iterator<Crop>(_vcrops.end());
}

my_const_iterator<Crop> Crops::begin() const
{
    return my_const_iterator<Crop>(_vcrops.begin());
}

my_const_iterator<Crop> Crops::end() const
{
    return my_const_iterator<Crop>(_vcrops.end());
}
///////////////////////////////////////////////////////////////////////////////
