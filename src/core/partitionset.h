
#pragma once
#include "geometry.h"

#include <vector>
#include <map>
#include <utility>

class Partitions;

class _2dTo1dConverter {
public:
    _2dTo1dConverter(const Rectangle& master_shape, const std::vector<Rectangle>& sub_shapes);
    _2dTo1dConverter(const _2dTo1dConverter& other) = delete;
    _2dTo1dConverter& operator=(const _2dTo1dConverter& other) = delete;

    /**
     * @brief Will return a pair of position and height (in range 0, 1). Position
     * will be negative if it is not possible to have the representation in only
     * one part.
     */
    std::pair<float, float> get_position(const Rectangle& rect);

private:
    const Shape& last_shape() const;
    void compute_y_positions(const Partitions& partitions);
    std::pair<float, float> compute_y_position(const Partitions& partitions, const Rectangle& rect);
    void clear_all_positions();

    std::map<Rectangle, std::pair<float, float> > _shapes;
    const Rectangle& _master_shape;
};
