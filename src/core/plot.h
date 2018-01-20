
#ifndef PLOT_H
#define PLOT_H

#include "geometry.h"

#include <string>
#include <memory>

class Plot
{
public:
    Plot();
    Plot(std::string name, std::string descr, std::unique_ptr< Shape > shape);
    Plot(std::string name, std::string descr="", float width=-1, float height=-1, float posx=-1, float posy=-1);
    Plot(const Plot& plot);
    ~Plot();
    void add_subplot(float width, float height, float posx, float posy);
    Shape& get_shape();
    const Shape& get_shape() const;
    void set_shape(std::unique_ptr< Shape > shape);
    void set_name(std::string new_name);
    const std::string& get_name() const;
    void set_note(std::string note);
    const std::string& get_note() const;

private:
    std::string name;
    std::string descr;
    std::unique_ptr< Shape > shape;
};

bool operator==(const Plot& lhs, const Plot& rhs);


#endif //PLOT_H
