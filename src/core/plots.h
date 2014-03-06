
#ifndef PLOTS_H
#define PLOTS_H

#include "geometry.h"
#include "keynames.h"

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
    void create_subplots(int nb_hor, int nb_vert);
    Rectangle get_rect();
    void set_rect(Rectangle rect);
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
    Plot& add_plot(string key, string name, string descr="", float width=-1, float height=-1, float posx=-1, float posy=-1);
    Plot& add_plot(Plot plot);
    void delete_plot(string key);
    void delete_plot(int index);
    void delete_plot(Plot& plot);
    Plot& get_plot(string key);
};

#endif //PLOTS_H
