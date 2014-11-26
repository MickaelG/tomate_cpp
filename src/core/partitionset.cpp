
#include "partitionset.h"

#include "crops.h"
#include "plot.h"

PartitionSet::PartitionSet(Plot &plot) :
    m_plot(plot)
{
}

void PartitionSet::compute_partitions(list<Crop*> l_crops)
{
    int vertical = 0;
    int horizontal = 0;
    Shape* plot_shape_p = m_plot.get_shape();

    for (Crop* crop_p: l_crops) {
        Shape* crop_shape_p = crop_p->get_shape();
        if (crop_shape_p->get_width() == plot_shape_p->get_width() &&
            crop_shape_p->get_height() == plot_shape_p->get_height()) {
            continue;
        }
        m_partitions.insert(Rectangle(*crop_shape_p));
        if (crop_shape_p->get_width() == plot_shape_p->get_width()) {
            horizontal += 1;
        }
        if (crop_shape_p->get_height() == plot_shape_p->get_height()) {
            vertical += 1;
        }
    }

    remove_partitions_overlaps(m_partitions);

    //Add partitions for empty parts of the plot
    set< Rectangle > empty_parts;
    Rectangle allpart(plot_shape_p->get_width(), plot_shape_p->get_height(), 0, 0);
    for (Rectangle realpart: m_partitions) {
        set<Rectangle> splitparts = get_split_partitions(realpart, allpart);
        empty_parts.insert(splitparts.begin(), splitparts.end());
    }
    remove_partitions_overlaps(empty_parts);

    set< Rectangle > empty_parts2;
    for (auto emptpart: empty_parts) {
        bool remove = false;
        for (auto realpart: m_partitions) {
            if (emptpart.is_inside(realpart)) {
                remove = true;
                continue;
            }
        }
        if (!remove) {
            empty_parts2.insert(emptpart);
        }
    }
    empty_parts = empty_parts2;

    m_partitions.insert(empty_parts.begin(), empty_parts.end());

    m_vertical = (vertical > horizontal);
}


void PartitionSet::remove_partitions_overlaps(set< Rectangle >& partitions)
{
    bool overlap = true;
    int loops = 0;
    while (overlap) {
        auto part1_it = partitions.begin();
        auto part2_it = partitions.begin();
        for (; part1_it != partitions.end(); ++part1_it) {
            Rectangle part1 = *part1_it;
            for (auto part2_it = partitions.begin(); part2_it != partitions.end(); ++part2_it) {
                Rectangle part2 = *part2_it;
                if (part1 == part2) {
                    continue;
                }
                if (part1.overlaps(part2)) {
                    overlap = true;
                    break;
                }
            }
            if (overlap) {
                break;
            }
        }
        if (overlap) {
            set< Rectangle > newparts = get_split_partitions(*part1_it, *part2_it);
            partitions.erase(part1_it);
            partitions.erase(part2_it);
            partitions.insert(newparts.begin(), newparts.end());
        }
        loops += 1;
        if (loops > 200) {
            cerr << "Too many loops in remove_partitions_overlaps" << endl;
            return;
        }
    }
}

list<Rectangle> PartitionSet::sort_partitions(list<Crop*> crops)
{
    map<Rectangle, set<Crop*> > crops_in_part;
    for (Rectangle partition: m_partitions) {
        for (Crop* crop_p: crops) {
            if (partition.is_inside(*crop_p->get_shape())) {
                crops_in_part[partition].insert(crop_p);
            }
        }
    }

    list< list< Rectangle> >grouped_partitions;
    //step 1: group parts that contain the same set of crops
    for (Rectangle part: m_partitions) {
        set<Crop*> mycrops = crops_in_part[part];
        if (mycrops.empty()) { // Part without any crop. we create a new
                               // group with this part only
            list<Rectangle> mylist;
            mylist.push_back(part);
            grouped_partitions.push_back(mylist);
            continue;
        }
        bool added = false;
        for (list<Rectangle> group: grouped_partitions) {
            Rectangle ref_part = group.front();
            if (crops_in_part[ref_part].empty()) {
                continue;
            }
            if (crops_in_part[ref_part] == crops_in_part[part]) {
                group.push_back(part);
                added = true;
                break;
            }
        }
        if (!added) {
            list<Rectangle> mylist;
            mylist.push_back(part);
            grouped_partitions.push_back(mylist);
        }
    }

    //step 2: order groups
    list<Rectangle> result = grouped_partitions.front();
    grouped_partitions.pop_front();
    while (!grouped_partitions.empty()) {
        if (crops_in_part[result.front()].empty()) {
            list<Rectangle> mylist = grouped_partitions.front();
            grouped_partitions.pop_front();
            result.insert(result.end(), mylist.begin(), mylist.end());
            continue;
        }
        auto groupit = grouped_partitions.begin();
        for (; groupit != grouped_partitions.end(); ++groupit) {
            // Another group has common crop with the current one. we put it
            // just after
            list<Rectangle> group = *groupit;
            set<Crop*> crops_in_group = crops_in_part[group.front()];
            set<Crop*> crops_in_last = crops_in_part[result.back()];
            vector<Crop*> common_crops;
            set_intersection(crops_in_group.begin(), crops_in_group.end(),
                             crops_in_last.begin(), crops_in_last.end(),
                             common_crops.begin());
            if (!common_crops.empty()) {
                break;
            }
        }

        if (groupit == grouped_partitions.end()) {
            list<Rectangle> mylist = grouped_partitions.front();
            grouped_partitions.pop_front();
            result.insert(result.end(), mylist.begin(), mylist.end());
        } else {
            list<Rectangle> mylist = *groupit;
            result.insert(result.end(), mylist.begin(), mylist.end());
            grouped_partitions.erase(groupit);
        }
    }

    return result;
}

list< pair<float, float> > compute_timerepr(const Crop& crop, const list<Rectangle>& partitions)
{
    list< pair<float, float> > y_coords;
    //TODO: handle split crops
    float timeoffset = 0;
    const Shape* plot_shape_p = crop.get_plot().get_shape();
    pair<float, float> prev_repr = make_pair(-1, 0);
    for (auto partition: partitions) {
        const Shape* crop_shape_p = crop.get_shape();
        if (partition.overlaps(*dynamic_cast<const Rectangle*>(crop_shape_p))) {
            float ypos = timeoffset / plot_shape_p->get_area();
            float height = partition.get_area() / plot_shape_p->get_area();
            pair<float, float> curr_repr = make_pair(ypos, height);

            if (abs(prev_repr.first + prev_repr.second - curr_repr.first) < 0.001) {
                curr_repr = make_pair(prev_repr.first, prev_repr.second + curr_repr.second);
                y_coords.pop_back();
                y_coords.push_back(curr_repr);
            } else {
                y_coords.push_back(curr_repr);
            }
            prev_repr = curr_repr;
        }
        timeoffset += partition.get_area();
    }
    return y_coords;
}
