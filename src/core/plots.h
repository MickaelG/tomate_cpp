
#ifndef PLOTS_H
#define PLOTS_H

#include <list>
#include <string>
using namespace std;

#include "plot.h"
class Crops;

//this is dangerous to store reference from vector elements. use list.
class Plots: public list<Plot>
{
public:
    Plots(const Crops& crops);
    const Plot &index(int data_index) const;
    Plot &index(int data_index);
    Plot& add_plot(string key, string name, string descr="", float width=-1, float height=-1, float posx=-1, float posy=-1);
    Plot& add_plot(Plot plot);
    void delete_plot(string key);
    void delete_plot(int index);
    void delete_plot(Plot& plot);
    Plot& get_plot(string key);
    bool is_used(int index) const;
    bool is_used(const Plot& plot) const;

private:
    const Crops& crops;
};

#endif //PLOTS_H
