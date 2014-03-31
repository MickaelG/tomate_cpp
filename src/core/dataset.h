

#ifndef DATASET_H
#define DATASET_H

#include <list>
#include <string>
#include <exception>
#include <stdexcept>
using namespace std;

#include <boost/date_time.hpp>
namespace bg = boost::gregorian;

#include "plots.h"
#include "plants.h"
#include "crops.h"


class Dataset
{
    private:
        string filename;
        Crops crops;
        Plots plots;
        Plants plants;
    public:
        Dataset();
        Crop& add_crop(Crop crop);
        Plant& add_plant(Plant plant);
        Plot& add_plot(Plot& plot);
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

#endif //DATASET_H
