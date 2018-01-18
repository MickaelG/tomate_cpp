
#ifndef PLOTS_H
#define PLOTS_H

#include <vector>
#include <string>
#include <memory>

#include "plot.h"
#include "my_iterator.h"
class Crops;

class Plots
{
public:
    Plots(const Crops& crops);
    Plot& add(const std::string& name, const std::string& descr="", float width=-1, float height=-1, float posx=-1, float posy=-1);
    void remove(int index);
    bool is_used(const Plot& plot) const;
    my_iterator<Plot> begin();
    my_iterator<Plot> end();
    my_const_iterator<Plot> begin() const;
    my_const_iterator<Plot> end() const;
    const Plot& operator[](int index) const;
    Plot& operator[](int index);
    int size() const;
    
    Plot* get_for_pos(const Shape& shape);

private:
    const Crops& crops;
    std::vector<std::unique_ptr< Plot > > _vplots;
};

#endif //PLOTS_H
