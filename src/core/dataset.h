

#ifndef DATASET_H
#define DATASET_H

#include <string>
using namespace std;

#include <boost/date_time.hpp>
namespace bg = boost::gregorian;

#include "plots.h"
#include "plants.h"
#include "crops.h"


class Dataset
{
public:
    Dataset();
    void set_filename(string in_filename);
    const Plots& get_plots() const;
    const Plants& get_plants() const;
    const Crops& get_crops() const;
    Plots& get_plots();
    Plants& get_plants();
    Crops& get_crops();
private:
    string _filename;
    Crops _crops;
    Plots _plots;
    Plants _plants;
};

#endif //DATASET_H
