
#include "partitionset.h"

#include "crops.h"
#include "plot.h"

#include <algorithm>
using namespace std;

static list<Rectangle> sort_partitions(set<Rectangle>& partitions, list<Crop*> crops, bool isvertical = true);
static bool remove_partitions_overlaps(set<Rectangle>& partitions);

template <typename T>
std::ostream& operator<<(std::ostream& stream, const std::list<T>& list)
{
    stream << "[";
    bool first = true;
    for (const T& item : list)
    {
        if (!first)
            stream << ", ";
        else
            first = false;
        stream << item;
    }
    stream << "]";
    return stream;
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const std::set<T>& list)
{
    stream << "{";
    bool first = true;
    for (const T& item : list)
    {
        if (!first)
            stream << ", ";
        else
            first = false;
        stream << item;
    }
    stream << "}";
    return stream;
}

list<Rectangle> compute_partitions(const list<Crop*>& l_crops, const Plot& plot)
{
    int vertical = 0;
    int horizontal = 0;
    const Shape* plot_shape_p = plot.get_shape();

    set<Rectangle> partitions;

    //Create a partition for each different crop
    for (Crop* crop_p: l_crops) {
        Shape* crop_shape_p = crop_p->get_shape();
        if (crop_shape_p->get_width() == plot_shape_p->get_width() &&
            crop_shape_p->get_height() == plot_shape_p->get_height()) {
            continue;
        }
        partitions.insert(Rectangle(*crop_shape_p));
        if (crop_shape_p->get_width() == plot_shape_p->get_width()) {
            horizontal += 1;
        }
        if (crop_shape_p->get_height() == plot_shape_p->get_height()) {
            vertical += 1;
        }
    }

    remove_partitions_overlaps(partitions);

    //Add partitions for empty parts of the plot
    set<Rectangle> empty_parts;
    Rectangle allpart(0, 0, plot_shape_p->get_width(), plot_shape_p->get_height());
    for (Rectangle realpart: partitions) {
        set<Rectangle> splitparts = compute_non_overlapping_rects(realpart, allpart);
        empty_parts.insert(splitparts.begin(), splitparts.end());
    }
    remove_partitions_overlaps(empty_parts);

    set< Rectangle > empty_parts2;
    for (auto emptpart: empty_parts) {
        bool remove = false;
        for (auto realpart: partitions) {
            if (emptpart.is_inside(realpart)) {
                remove = true;
                break;
            }
        }
        if (!remove) {
            empty_parts2.insert(emptpart);
        }
    }
    empty_parts = empty_parts2;

    partitions.insert(empty_parts.begin(), empty_parts.end());

    list<Rectangle> lpartitions = sort_partitions(partitions, l_crops, (vertical>horizontal));

    return lpartitions;
}

bool is_before_vert(const Rectangle &a, const Rectangle &b)
{
    if (a.get_min_x() < b.get_min_x()) return true;
    if (a.get_min_x() == b.get_min_x() && a.get_min_y() < b.get_min_y()) return true;
    if (a.get_min_x() == b.get_min_x() && a.get_min_y() == b.get_min_y() && a.get_width() < b.get_width()) return true;
    if (a.get_min_x() == b.get_min_x() && a.get_min_y() == b.get_min_y() && a.get_width() == b.get_width() && a.get_height() < b.get_height()) return true;
    return false;
}

bool is_before_hori(const Rectangle &a, const Rectangle &b)
{
    if (a.get_min_y() < b.get_min_y()) return true;
    if (a.get_min_y() == b.get_min_y() && a.get_min_x() < b.get_min_x()) return true;
    if (a.get_min_y() == b.get_min_y() && a.get_min_x() == b.get_min_x() && a.get_height() < b.get_height()) return true;
    if (a.get_min_y() == b.get_min_y() && a.get_min_x() == b.get_min_x() && a.get_height() == b.get_height() && a.get_width() < b.get_width()) return true;
    return false;
}

static bool remove_partitions_overlaps(set<Rectangle>& partitions)
{
    bool overlap = true;
    int loops = 0;
    while (overlap) {
        overlap = false;
        auto part1_it = partitions.begin();
        set<Rectangle>::iterator part2_it;
        for (; part1_it != partitions.end(); ++part1_it) {
            const Rectangle& part1(*part1_it);
            for (part2_it = partitions.begin(); part2_it != partitions.end(); ++part2_it) {
                const Rectangle& part2(*part2_it);
                if (&part1 == &part2) {
                    continue;
                }
                if (part1.overlaps(part2)) {
                    //cout << "DEBUG: overlap found between " << part1 << " and " << part2 << endl;
                    overlap = true;
                    break;
                }
            }
            if (overlap) {
                break;
            }
        }
        if (overlap) {
            const Rectangle& part1 = *part1_it;
            const Rectangle& part2 = *part2_it;
            set< Rectangle > newparts = compute_non_overlapping_rects(part1, part2);
            partitions.erase(part1);
            partitions.erase(part2);
            partitions.insert(newparts.begin(), newparts.end());
        }
        loops += 1;
        if (loops > 200) {
            cerr << "Too many loops in remove_partitions_overlaps" << endl;
            return false;
        }
    }
    return true;
}

static list<Rectangle> sort_partitions(set<Rectangle>& partitions, list<Crop*> crops, bool isvertical)
{
    map<Rectangle, set<Crop*> > crops_in_part;
    for (Rectangle partition: partitions) {
        for (Crop* crop_p: crops) {
            if (partition.is_inside(*crop_p->get_shape())) {
                crops_in_part[partition].insert(crop_p);
            }
        }
    }

    //First default sort
    list<Rectangle> lpartitions(partitions.begin(), partitions.end());
    if (isvertical) {
      lpartitions.sort(is_before_vert);
    } else {
      lpartitions.sort(is_before_hori);
    }

    list< list< Rectangle> >grouped_partitions;
    //step 1: group parts that contain the same set of crops
    for (Rectangle part: lpartitions) {
        set<Crop*> mycrops = crops_in_part[part];
        if (mycrops.empty()) { // Part without any crop. we create a new
                               // group with this part only
            list<Rectangle> mylist;
            mylist.push_back(part);
            grouped_partitions.push_back(mylist);
            continue;
        }
        bool added = false;
        for (list<Rectangle>& group: grouped_partitions) {
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
    if (grouped_partitions.empty()) {
        list<Rectangle> empty_result;
        return empty_result;
    }
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
                             back_inserter(common_crops));
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
