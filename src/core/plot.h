
#ifndef PLOT_H
#define PLOT_H

#include "geometry.h"
#include "keynames.h"

class Plot: public KeyName
{
private:
    Shape* shape;
    list<Plot> subplots;
public:
    Plot();
    Plot(string key, string name, string descr, Shape *shape);
    Plot(string key, string name, string descr="", float width=-1, float height=-1, float posx=-1, float posy=-1);
    Plot(const Plot& plot);
    ~Plot();
    void add_subplot(float width, float height, float posx, float posy);
    Shape *get_shape();
    void set_shape(Shape *shape);
};
extern const Plot NullPlot;

#endif //PLOT_H
