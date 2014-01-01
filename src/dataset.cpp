#include "dataset.h"


Rectangle::Rectangle(int width, int height, int posx, int posy) :
    width(width), height(height), posx(posx), posy(posy) {}


Rectangle::Rectangle()
{
    width = 0;
    height = 0;
    posx = 0;
    posy = 0;
}


/*
template <class T> T& ListKeyNames<T>::index(int data_index)
{
    if (data_index >= 0)
    {
        typename list<T>::iterator it = this->begin();
        advance(it, data_index);
        return *it;
    }
    else
    {
        return NullKeyName;
    }
}
*/

KeyName NullKeyName;
KeyName& KeyNames::index(int data_index)
{
    if (data_index >= 0)
    {
        KeyNames::iterator it = this->begin();
        advance(it, data_index);
        return *it;
    }
    else
    {
        return NullKeyName;
    }
}

Plot& Plots::index(int data_index)
{
    if (data_index >= 0)
    {
        Plots::iterator it = this->begin();
        advance(it, data_index);
        return *it;
    }
    else
    {
        return NullPlot;
    }
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

Var::Var(string key, string name, string note) :
    KeyName(key, name), note(note) {}

Plot NullPlot;
Plot::Plot(string key, string name, string descr, Rectangle rect) :
    KeyName(key, name), geometry(rect), descr(descr) {}

Plot::Plot(string key, string name, string descr, float width, float height, float posx, float posy) :
    KeyName(key, name), descr(descr)
{
    geometry = Rectangle(width, height, posx, posy);
}

void Plot::add_subplot(float width, float height, float posx, float posy)
{
    int key_num = subplots.size() + 1;
    string subd_key = key + "-" + to_string(key_num);
    subplots.push_back(Plot(subd_key, "", "", width, height, posx, posy));
}

Crop NullCrop;
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

void Crop::set_note(string note)
{
    this->note = note;
}

Crop::Crop() : p_plant(0), p_plot(0)
{
}

Crop::Crop(bg::date start_date, bg::date end_date,
     bg::date planned_start_date, bg::date planned_end_date,
     Plant &plant, string varkey,
     Plot &plot, string note) :
    start_date(start_date), end_date(end_date),
    planned_start_date(planned_start_date), planned_end_date(planned_end_date),
    varkey(varkey),
    p_plant(&plant), p_plot(&plot), note(note)
{
}

Crop::Crop(bg::date start_date, bg::date end_date,
     Plant &plant, string varkey,
     Plot &plot, string note) :
    start_date(start_date), end_date(end_date),
    varkey(varkey),
    p_plant(&plant), p_plot(&plot), note(note)
{
}

string Crop::description() const
{
    return get_plant().get_name() + " start:" + bg::to_simple_string(get_date("start")) +
                                    " end:"   + bg::to_simple_string(get_date("end")) +
                                    " planned_start:" + bg::to_simple_string(get_date("planned_start")) +
                                    " planned_end:"   + bg::to_simple_string(get_date("planned_end"));
}

Plant& Crop::get_plant() const
{
    if (p_plant)
    {
        return *p_plant;
    } else {
        return NullPlant;
    }
};

void Crop::set_plant(Plant& plant)
{
    p_plant = &plant;
}

Plot& Crop::get_plot() const
{
    if (p_plot)
    {
        return *p_plot;
    } else {
        return NullPlot;
    }
};

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

string Crop::get_note() const
{
    return note;
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
    if ((~start_date.is_not_a_date() && date >= start_date) && 
        (~get_virtual_end_date().is_not_a_date() && date <= get_virtual_end_date()))
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
    if ((~planned_start_date.is_not_a_date() && date > planned_start_date) &&
        (~planned_end_date.is_not_a_date() && date < planned_end_date))
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
    if ((~get_virtual_end_date().is_not_a_date() && get_virtual_end_date() >= date) &&
        (~start_date.is_not_a_date() && start_date <= date + bg::date_duration(365)))
    {
        return true;
    }
    else if ((~planned_end_date.is_not_a_date() && planned_end_date >= date) &&
             (~planned_start_date.is_not_a_date() && planned_start_date <= date + bg::date_duration(365)))
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


Crop& Crops::find_crop(const Plot& plot, bg::date date)
{
    //for (int i_crop = 0; i_crop < this->size(); i_crop++)
    for (Crops::iterator it=this->begin(); it != this->end(); ++it)
    {
        //TODO: add operator== for Plot
        if (it->get_plot().get_key() == plot.get_key())
        {
           if (it->is_active_at_date(date) || it->is_planned_at_date(date))
           {
               return *it;
           }
        }
    }
    return NullCrop;
}

Crop& Dataset::add_crop(Crop crop)
{
    this->crops.push_back(crop);
    return crops.back();
}

Plot& Dataset::add_plot(string key, string name, string descr, float width, float height, float posx, float posy)
{
    this->plots.push_back(Plot(key, name, descr, width, height, posx, posy));
    return plots.back();
}
Plot& Dataset::add_plot(Plot plot)
{
    this->plots.push_back(plot);
    return plots.back();
}

Plant& Dataset::add_plant(Plant plant)
{
    plants.push_back(plant);
    return plants.back();
};

Plant& Dataset::get_plant(string key)
{
    return get_plants().get_plant(key);
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

void Dataset::set_filename(string in_filename)
{
    filename = in_filename;
}

const Plots& Dataset::get_plots() const
{
    return plots;
}

const Plants& Dataset::get_plants() const
{
    return plants;
}

const Crops& Dataset::get_crops() const {
    return crops;
}

Plots& Dataset::get_plots() {
    return plots;
}

Plants& Dataset::get_plants() {
    return plants;
}

Crops& Dataset::get_crops() {
    return crops;
}

Plant NullPlant;
Plant::Plant(string key, string name, string note, string color) :
    KeyName(key, name), note(note), color(color) {};
    
void Plant::add_var(string key, string name, string note)
{
    if (key == "")
    {
        key = to_string(varlist.size());
    }
    //TODO : check that key is unique
    varlist.push_back(Var(key, name, note));
}

string Plant::get_note() const
{
    return note;
}

void Plant::set_note(string in_note)
{
    note = in_note;
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


//TODO: search hierarchically without running through all subplots
Plot& Dataset::get_plot(string key)
{
    for (Plots::iterator it=plots.begin(); it != plots.end(); ++it)
    {
        if (it->get_key() == key)
        {
            return *it;
        }
        if (it->get_subplot(key))
        {
            return it->get_subplot(key);
        }
    }
    return NullPlot;
}

Plot& Plot::get_subplot(string key)
{
    for (Plots::iterator it=subplots.begin(); it != subplots.end(); ++it)
    {
        if (it->get_key() == key)
        {
            return *it;
        }

    }
    return NullPlot;
}
