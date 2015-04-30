
#ifndef PLOT_H
#define PLOT_H

#include "geometry.h"
#include "keynames.h"

class Plot: public KeyName
{
public:
    Plot();
    Plot(std::string key, std::string name, std::string descr, Shape *shape);
    Plot(std::string key, std::string name, std::string descr="", float width=-1, float height=-1, float posx=-1, float posy=-1);
    Plot(const Plot& plot);
    ~Plot();
    void add_subplot(float width, float height, float posx, float posy);
    Shape* get_shape();
    const Shape* get_shape() const;
    void set_shape(Shape *shape);

private:
    Shape* shape;
};

#endif //PLOT_H
