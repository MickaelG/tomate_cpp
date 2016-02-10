
#include "partitionset.h"

#include "crops.h"
#include "plot.h"
#include "log.h"

#include <algorithm>
#include<list>
using namespace std;


class Partitions {

public:
    Partitions(const vector<Rectangle>& sub_shapes, const Rectangle& master_shape);
    bool empty() const;
    const vector<Rectangle>& get_sorted() const;

private:
    void sort(const vector<Rectangle>& sub_shapes);
    void push_sorted(const vector<Rectangle>& elems);
    const Rectangle& last_sorted() const;

    set<Rectangle> _partitions;
    vector<Rectangle> _sorted_partitions;
    int _horizontal;
};

bool Partitions::empty() const
{
    return _sorted_partitions.empty();
}

_2dTo1dConverter::_2dTo1dConverter(const Rectangle& master_shape, const vector<Rectangle>& sub_shapes)
  : _master_shape(master_shape)
{
    for (auto& rect: sub_shapes) {
        _shapes.insert(make_pair(rect, make_pair(-1, -1)));
    }

    Partitions partitions(sub_shapes, master_shape);
    compute_y_positions(partitions);
}

pair<float, float> _2dTo1dConverter::get_position(const Rectangle& rect)
{
    auto found = _shapes.find(rect);
    if (found == _shapes.end()) {
        throw logic_error("get_position called on unknown rect");
    }
    return found->second;
}

static bool remove_overlaps(set<Rectangle>& partitions)
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
        if (loops > 2000) {
            Log::Error("Too many loops in Partitions::remove_overlaps");
            return false;
        }
    }
    return true;
}

static bool is_before_vert(const Rectangle &a, const Rectangle &b)
{
    if (a.get_min_x() < b.get_min_x()) return true;
    if (a.get_min_x() == b.get_min_x() && a.get_min_y() < b.get_min_y()) return true;
    if (a.get_min_x() == b.get_min_x() && a.get_min_y() == b.get_min_y() && a.get_width() < b.get_width()) return true;
    if (a.get_min_x() == b.get_min_x() && a.get_min_y() == b.get_min_y() && a.get_width() == b.get_width() && a.get_height() < b.get_height()) return true;
    return false;
}

static bool is_before_hori(const Rectangle &a, const Rectangle &b)
{
    if (a.get_min_y() < b.get_min_y()) return true;
    if (a.get_min_y() == b.get_min_y() && a.get_min_x() < b.get_min_x()) return true;
    if (a.get_min_y() == b.get_min_y() && a.get_min_x() == b.get_min_x() && a.get_height() < b.get_height()) return true;
    if (a.get_min_y() == b.get_min_y() && a.get_min_x() == b.get_min_x() && a.get_height() == b.get_height() && a.get_width() < b.get_width()) return true;
    return false;
}

void Partitions::sort(const vector<Rectangle>& sub_shapes)
{
    map<Rectangle, set<const Rectangle*> > crops_in_part;
    for (Rectangle partition: _partitions) {
        for (const Rectangle& sub_shape: sub_shapes) {
            if (partition.is_inside(sub_shape)) {
                crops_in_part[partition].insert(&sub_shape);
            }
        }
    }

    //First default sort
    list<Rectangle> lpartitions(_partitions.begin(), _partitions.end());
    if (_horizontal < 0) {
      lpartitions.sort(is_before_vert);
    } else {
      lpartitions.sort(is_before_hori);
    }

    list< vector< Rectangle> >grouped_partitions;
    //step 1: group parts that contain the same set of crops
    for (Rectangle part: lpartitions) {
        auto mycrops = crops_in_part[part];
        if (mycrops.empty()) { // Part without any crop. we create a new
                               // group with this part only
            vector<Rectangle> mylist;
            mylist.push_back(part);
            grouped_partitions.push_back(mylist);
            continue;
        }
        bool added = false;
        for (auto& group: grouped_partitions) {
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
            vector<Rectangle> mylist;
            mylist.push_back(part);
            grouped_partitions.push_back(mylist);
        }
    }

    //step 2: order groups
    if (grouped_partitions.empty()) {
        return;
    }

    push_sorted(grouped_partitions.front());
    grouped_partitions.pop_front();

    while (!grouped_partitions.empty()) {
        auto next_group_it = grouped_partitions.begin();
        set<const Rectangle*> crops_in_last = crops_in_part[last_sorted()];
        for (auto groupit = grouped_partitions.begin(); groupit != grouped_partitions.end(); ++groupit) {
            // Another group has common crop with the current one. we put it
            // just after
            auto group = *groupit;
            set<const Rectangle*> crops_in_group = crops_in_part[group.back()];
            vector<const Rectangle*> common_crops;
            set_intersection(crops_in_group.begin(), crops_in_group.end(),
                             crops_in_last.begin(), crops_in_last.end(),
                             back_inserter(common_crops));
            if (!common_crops.empty()) {
                next_group_it = groupit;
                break;
            }

        }

        /*
        vector<const Crop*> new_crops;
        set_difference(crops_in_group.begin(), crops_in_group.end(),
                       crops_in_last.begin(), crops_in_last.end(),
                       back_inserter(new_crops));
        for (auto crop: crops_in_last) {
            if (crops_in_group.count(crop) == 0) {
                finished_crops.insert(crop);
            }
        }
        */

        push_sorted(*next_group_it);
        grouped_partitions.erase(next_group_it);
    }
}

const vector<Rectangle>& Partitions::get_sorted() const
{
    return _sorted_partitions;
}

void Partitions::push_sorted(const vector<Rectangle>& elems)
{
    for(const Rectangle& rect: elems) {
        _sorted_partitions.push_back(rect);
    }
}

const Rectangle& Partitions::last_sorted() const
{
    if (_sorted_partitions.empty()) {
        throw logic_error("_sorted_partitions accessed whereas empty");
    }
    return _sorted_partitions.back();
}

Partitions::Partitions(const vector<Rectangle> &sub_shapes, const Rectangle& master_shape) :
    _horizontal(0)
{
    //Create a partition for each different crop
    for (const Rectangle& subrect: sub_shapes) {
        if (subrect == master_shape) {
            continue;
        }
        _partitions.insert(Rectangle(subrect));
        if (subrect.get_width() == master_shape.get_width()) {
            _horizontal += 1;
        }
        if (subrect.get_height() == master_shape.get_height()) {
            _horizontal -= 1;
        }
    }

    remove_overlaps(_partitions);

    //Add partitions for empty parts of the plot
    set<Rectangle> empty_parts;
    Rectangle allpart(0, 0, master_shape.get_width(), master_shape.get_height());
    for (Rectangle realpart: _partitions) {
        set<Rectangle> splitparts = compute_non_overlapping_rects(realpart, allpart);
        empty_parts.insert(splitparts.begin(), splitparts.end());
    }
    remove_overlaps(empty_parts);

    set< Rectangle > empty_parts2;
    for (const Rectangle& emptpart: empty_parts) {
        bool insert = true;
        for (auto realpart: _partitions) {
            if (emptpart.is_inside(realpart)) {
                insert = false;
                break;
            }
        }
        if (insert) {
            empty_parts2.insert(emptpart);
        }
    }
    empty_parts = empty_parts2;

    _partitions.insert(empty_parts.begin(), empty_parts.end());

    sort(sub_shapes);
}

pair<float, float> _2dTo1dConverter::compute_y_position(const Partitions& partitions, const Rectangle& rect)
{
    float timeoffset = 0;
    float total_height = rect.get_area() / _master_shape.get_area();
    float computed_height = 0;
    float computed_pos = 0;
    for (auto partition: partitions.get_sorted()) {
        if (partition.overlaps(rect)) {
            float ypos = timeoffset / _master_shape.get_area();
            float height = partition.get_area() / _master_shape.get_area();

            if (computed_height == 0) {
                computed_pos = ypos;
                computed_height = height;
            } else if (abs(computed_pos + computed_height - ypos) < 0.0001) {
                computed_height += height;
            } else {
                //No representation possible with a single element
                return make_pair(-1, total_height);
            }
        }
        timeoffset += partition.get_area();
    }
    if (!partitions.empty() && abs (computed_height - total_height) > 0.0001) {
        Log::Warning("wrong height for rect " + rect.str() +
                     " (computed: " + to_string(computed_height) +
                     ", ref:" + to_string(total_height) + ")");
    }
    return make_pair(computed_pos, total_height);
}

void _2dTo1dConverter::compute_y_positions(const Partitions& partitions)
{
    bool invalid_pos = false;
    for (auto& rect_res: _shapes) {
        pair<float, float>& pos = rect_res.second;
        pos = compute_y_position(partitions, rect_res.first);
        if (pos.first == -1) {
            invalid_pos = true;
        }
    }
    if (invalid_pos) {
        clear_all_positions();
    }
}

void _2dTo1dConverter::clear_all_positions()
{
    for (auto& rect_res: _shapes) {
        pair<float, float>& pos = rect_res.second;
        pos.first = -1;
    }
}
