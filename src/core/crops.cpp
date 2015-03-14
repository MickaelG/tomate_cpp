
#include "crops.h"

#include "plot.h"
#include "plant.h"

#include <stdexcept>

///////////////////////////////////////////////////////////////////////////////
// class Crop
///////////////////////////////////////////////////////////////////////////////
const Crop NullCrop;
string Crop::str_descr() const
{
    return p_plant->get_name();
}

bg::date Crop::get_date(string which) const
{
    if (which == "planned_start")
    {
        return planned_start_date;
    }
    else if (which == "planned_end")
    {
        return planned_end_date;
    }
    else if (which == "end")
    {
        return end_date;
    }
    else if (which == "start")
    {
        return start_date;
    } else {
        throw "Erreur, unknown which";
    }
}

void Crop::set_date(string which, bg::date date)
{
    if (which == "planned_start")
    {
        planned_start_date = date;
    }
    else if (which == "planned_end")
    {
        planned_end_date = date;
    }
    else if (which == "end")
    {
       end_date = date;
    }
    else if (which == "start")
    {
        start_date = date;
    } else {

    }
}

Crop::Crop() : p_plant(0), p_plot(0), shape(0)
{
}

Crop::Crop(bg::date start_date, bg::date end_date,
     bg::date planned_start_date, bg::date planned_end_date,
     Plant* p_plant, string varkey,
     Plot* p_plot, string note, Rectangle rect) :
    start_date(start_date), end_date(end_date),
    planned_start_date(planned_start_date), planned_end_date(planned_end_date),
    varkey(varkey),
    p_plant(p_plant), p_plot(p_plot), note(note), shape(0)
{
    if (! (rect.get_width() < 0))
    {
        set_shape(new Rectangle(rect));
    }
}

Crop::Crop(bg::date start_date, bg::date end_date,
     Plant* p_plant, string varkey,
     Plot* p_plot, string note) :
    start_date(start_date), end_date(end_date),
    varkey(varkey),
    p_plant(p_plant), p_plot(p_plot), note(note), shape(0)
{
}

string Crop::description() const
{
    return get_plant().get_name() + " start:" + bg::to_simple_string(get_date("start")) +
                                    " end:"   + bg::to_simple_string(get_date("end")) +
                                    " planned_start:" + bg::to_simple_string(get_date("planned_start")) +
                                    " planned_end:"   + bg::to_simple_string(get_date("planned_end"));
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
    if (p_plant)
    {
        return *p_plant;
    } else {
        return NullPlant;
    }
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
    if (p_plot)
    {
        return *p_plot;
    } else {
        throw logic_error("Plot is not yet defined for crop");
    }
}

const Plot& Crop::get_plot() const
{
    if (p_plot)
    {
        return *p_plot;
    } else {
        return NullPlot;
    }
}

void Crop::set_plot(Plot& plot)
{
    p_plot = &plot;
}

void Crop::add_action(bg::date date, string note)
{
    actions.push_back(CropAction(date, note));
}

list<CropAction>& Crop::get_actions()
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
    if (!shape)
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
    return shape;
}

const Shape* Crop::get_shape() const
{
    return shape;
}

void Crop::set_shape(Shape* in_shape)
{
    if (shape)
    {
        delete shape;
        shape = NULL;
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
vector<Crop*> Crops::find_crops(const Plot& plot, bg::date date)
{
    vector<Crop*> result;
    //for (int i_crop = 0; i_crop < this->size(); i_crop++)
    for (Crops::iterator it=this->begin(); it != this->end(); ++it)
    {
        if (it->get_plot() == plot)
        {
           if (it->is_active_at_date(date) || it->is_planned_at_date(date))
           {
               result.push_back(&*it);
           }
        }
    }
    return result;
}

bool Crops::is_used_plot(const Plot& plot) const
{
    string plot_key = plot.get_key();
    for (Crops::const_iterator it=this->begin(); it != this->end(); ++it)
    {
        string loop_key = it->get_plot().get_key();
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
    for (Crops::const_iterator it=this->begin(); it != this->end(); ++it)
    {
        string loop_key = it->get_plant().get_key();
        //plant_key must be conntained in loop_key
        if (loop_key.find(plant_key) == 0)
        {
            return true;
        }
    }
    return false;
}

bool Crops::delete_crop(Crop& crop)
{
    remove(crop);
    return true;
}
///////////////////////////////////////////////////////////////////////////////
