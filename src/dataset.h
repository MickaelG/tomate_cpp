

#ifndef DATASET_H
#define DATASET_H

#include <list>
#include <string>
#include <exception>
#include <stdexcept>
using namespace std;

#include <boost/date_time.hpp>
namespace bg = boost::gregorian;


class Rectangle
{
    private:
        int width, height;
        int posx, posy;
    public:
        Rectangle();
        Rectangle(int, int, int, int);
        float get_width() const { return width; }
        float get_height() const { return height; }
        float get_x() const { return posx; }
        float get_y() const { return posy; }
        explicit operator bool() const { return !(posx == -1); };
};


class KeyName
{
    protected:
        string key;
        string name;
        string note;
    public:
        KeyName();
        KeyName(string key, string name, string note);
        string get_key() const;
        string get_name() const;
        string get_note() const;
        void set_note(string);
        void set_name(string name);
        explicit operator bool() const;
};
extern KeyName NullKeyName;

/*
//this is dangerous to store reference from vector elements. use list.
template <class T> class ListKeyNames: public list<T>
{
public:
    T& index(int data_index);
};
*/

class KeyNames: public list<KeyName>
{
public:
    KeyName& index(int data_index);
};


class Plot: public KeyName
{
    private:
        Rectangle geometry;
        list<Plot> subplots;
    public:
        Plot();
        Plot(string key, string name, string descr, Rectangle rect);
        Plot(string key, string name, string descr="", float width=-1, float height=-1, float posx=-1, float posy=-1);
        void add_subplot(float width, float height, float posx, float posy);
        Rectangle& get_rect();
        const list<Plot>& get_subplots() const;
        list<Plot>& get_subplots();
        Plot& get_subplot(string key);
};
extern Plot NullPlot;

//this is dangerous to store reference from vector elements. use list.
class Plots: public list<Plot>
{
public:
    Plot& index(int data_index);
};


class Var: public KeyName
{
    private:
    public:
        Var(string key, string name, string note = "");
};

class Vars: public list<Var>
{
};


class Plant: public KeyName
{
    private:
        string color;
        Vars varlist;
    public:
        Plant() {};
        Plant(string key, string name, string note="", string color="");
        void add_var(string key, string name, string note="");
        string get_color_str() const;
        void set_color_str(string color);
        Vars& get_vars();
};
extern Plant NullPlant;

//this is dangerous to store reference from vector elements. use list.
class Plants: public list<Plant>
{
public:
    Plant& index(int data_index);
    Plant& add_plant(string key, string name);
    Plant& get_plant(string key);
};


class CropAction
{
    private:
        bg::date date;
        string note;
    public:
        CropAction(bg::date date, string note) : date(date), note(note) {};
        bg::date get_date() const { return date; }
        string get_note() const { return note; }
};

class Crop
{
    private:
        bg::date start_date;
        bg::date end_date;
        bg::date planned_start_date;
        bg::date planned_end_date;
        string note;
        Plant* p_plant;
        string varkey;
        Plot* p_plot;
        list<CropAction> actions;
    public:
        Crop();
        Crop(bg::date start_date, bg::date end_date,
             bg::date planned_start_date, bg::date planned_end_date,
             Plant &plant, string varkey,
             Plot &plot, string note = "");
        Crop(bg::date start_date, bg::date end_date,
             Plant &plant, string varkey,
             Plot &plot, string note = "");
        string str_descr() const;
        Plant& get_plant() const;
        void set_plant(Plant& plant);
        Plot& get_plot() const;
        void set_plot(Plot& plot);
        bg::date get_date(string which) const;
        void set_date(string which, bg::date date);
        bg::date get_virtual_end_date() const;
        bg::date get_virtual_planned_end_date() const;
        void add_action(bg::date date, string note);
        list<CropAction>& get_actions();
        string get_varkey() const;
        void set_varkey(string varkey);
        string get_note() const;
        void set_note(string note);
        explicit operator bool() const;
        bool is_active_at_date(bg::date date) const;
        bool is_planned_at_date(bg::date date) const;
        bool is_in_year_started_by(bg::date date) const;
        string description() const;
};
extern Crop NullCrop;


class Crops: public list<Crop>
{
    private:
    public:
        Crop& find_crop(const Plot& plot, bg::date date);
};


class Dataset
{
    private:
        string filename;
        Plots plots;
        Crops crops;
        Plants plants;
    public:
        Dataset() {};
        Plot& add_plot(Plot plot);
        Plot& add_plot(string key, string name, string descr, float width, float height, float posx, float posy);
        Crop& add_crop(Crop crop);
        Plant& add_plant(Plant plant);
        void set_filename(string in_filename);
        Plant& get_plant(string key);
        Plot& get_plot(string key);
        const Plots& get_plots() const;
        const Plants& get_plants() const;
        const Crops& get_crops() const;
        Plots& get_plots();
        Plants& get_plants();
        Crops& get_crops();
};

int xml_read_data(string filename, Dataset& dataset);
int xml_write_data(string filename,const Dataset& dataset);

#endif //DATASET_H
