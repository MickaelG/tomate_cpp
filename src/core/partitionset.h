
#pragma once
#include "geometry.h"

#include <list>
using namespace std;

class Plot;
class Crop;

class PartitionSet
{
public:
    PartitionSet(Plot& plot);
    void compute_partitions(list<Crop*> l_crops);
    void remove_partitions_overlaps(set<Rectangle>& partitions);
    list<Rectangle> sort_partitions(list<Crop*> crops);

private:
    Plot& m_plot;
    set< Rectangle > m_partitions;
    bool m_vertical;
};

list< pair<float, float> > compute_timerepr(const Crop& crop, const list<Rectangle>& partitions);
