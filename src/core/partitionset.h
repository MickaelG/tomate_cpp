
#pragma once
#include "geometry.h"

#include <list>
using namespace std;

class Plot;
class Crop;


list< pair<float, float> > compute_timerepr(const Crop& crop, const list<Rectangle>& partitions);
list<Rectangle> compute_partitions(const list<Crop*>& l_crops, const Plot& plot);

bool is_before_vert(const Rectangle &a, const Rectangle &b);
bool is_before_hori(const Rectangle &a, const Rectangle &b);
