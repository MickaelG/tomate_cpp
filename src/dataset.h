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
        float get_posx () const { return posx; }
        float get_posy () const { return posy; }
};


class KeyName
{
    protected:
        string key;
        string name;
    public:
        KeyName() {};
        KeyName(string key, string name) : key(key), name(name) {};
        string get_key() const { return key; };
        string get_name() const { return name; };
        void set_name(string in_name) { name = in_name; }
        explicit operator bool() const { return !(key == "" && name == ""); };
};


class Plot: public KeyName
{
    private:
        Rectangle geometry;
        string descr;
        vector<Plot> subplots;
    public:
        Plot() {};
        Plot(string key, string name, string descr, Rectangle rect);
        Plot(string key, string name, string descr="", float width=-1, float height=-1, float posx=-1, float posy=-1);
        void add_subplot(float width, float height, float posx, float posy);
        string get_descr() const { return descr; };
        Rectangle& get_rect() { return geometry; };
        vector<Plot>& get_subplots() { return subplots; };
        Plot& get_subplot(string key);
};
extern Plot NullPlot;

class Var: public KeyName
{
    private:
        string note;
    public:
        Var(string key, string name, string note);
        string get_note() const { return note; }
};
class Plant: public KeyName
{
    private:
        string color;
        string note;
        vector<Var> varlist;
    public:
        Plant() {};
        Plant(string key, string name, string note="", string color="");
        void add_var(string key, string name, string note) { varlist.push_back(Var(key, name, note)); }
        string get_note() const { return note; }
        string get_color_str() const { return color; }
        vector<Var>& get_vars() { return varlist; }
};
extern Plant NullPlant;


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
        Plant &plant;
        string varkey;
        Plot &plot;
        list<CropAction> actions;
    public:
        Crop(bg::date start_date, bg::date end_date,
             bg::date planned_start_date, bg::date planned_end_date,
             Plant &plant, string varkey,
             Plot &plot, string note = "") :
            start_date(start_date), end_date(end_date),
            planned_start_date(planned_start_date), planned_end_date(planned_end_date),
            varkey(varkey),
            plant(plant), plot(plot), note(note) {};
        Crop(bg::date start_date, bg::date end_date,
             Plant &plant, string varkey,
             Plot &plot, string note = "") :
            start_date(start_date), end_date(end_date),
            varkey(varkey),
            plant(plant), plot(plot), note(note) {};
        string str_descr() const;
        Plant& get_plant() const { return plant; };
        Plot& get_plot() const { return plot; };
        bg::date get_date(string which) const;
        void add_action(bg::date date, string note) { actions.push_back(CropAction(date, note)); }
        list<CropAction>& get_actions() { return actions; }
        string get_varkey() const { return varkey; }
        string get_note() const { return note; }
};


class Dataset
{
    private:
        string filename;
        vector<Plot> plots;
        list<Crop> crops;
        vector<Plant> plants;
    public:
        Dataset() {};
        void add_plot(Plot plot);
        void add_plot(string key, string name, string descr, float width, float height, float posx, float posy);
        void add_crop(Crop crop);
        void add_plant(Plant plant) { plants.push_back(plant); };
        void set_filename(string in_filename) { filename = in_filename; };
        Plant& get_plant(string key);
        Plot& get_plot(string key);
        const vector<Plot>& get_plots() const { return plots; };
        const vector<Plant>& get_plants() const { return plants; };
        const list<Crop>& get_crops() const { return crops; };
};

int xml_read_data(string filename, Dataset& dataset);
int xml_write_data(string filename,const Dataset& dataset);

