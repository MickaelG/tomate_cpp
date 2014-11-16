#!/usr/bin/python3

"""
Prototype script for spaceview to timeview conversion
"""

import sys
import svgwrite

class Rectangle:
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height

    @property
    def x2(self):
        return self.x + self.width

    @property
    def y2(self):
        return self.y + self.height

    @property
    def area(self):
        return self.width * self.height

    @property
    def tuple(self):
        return (self.x, self.y, self.width, self.height)

    def __getitem__(self, idx):
        if idx == 0:
            return self.x
        elif idx == 1:
            return self.y
        elif idx == 2:
            return self.width
        elif idx == 3:
            return self.height

    def __eq__(self, other):
        return self.tuple == other.tuple

    def is_inside(self, other):
        for dim in (0, 1):
            if self[0+dim] < other[0+dim]:
                return False
            if self[0+dim] + self[2+dim] > other[0+dim] + other[2+dim]:
                return False
        return True


class Partition(Rectangle):
    def __init__(self, x, y, width, height):
        Rectangle.__init__(self, x, y, width, height)

    def __lt__(self, other):
        return (self.y, self.x) < (other.y, other.x)

    def __hash__(self):
        return hash(self.__repr__())

    def __repr__(self):
        return "Partition(%s, %s, %s, %s)" % (self.x, self.y, self.width, self.height)

    def overlaps(self, other):
        for dim in (0, 1):
          if self[0+dim] >= other[0+dim] + other[2+dim]:
              return False
          if other[0+dim] >= self[0+dim] + self[2+dim]:
              return False
        return True

    def intersection(self, other):
        assert self.overlaps(other)
        (x, y) = (max(self.x, other.x), max(self.y, other.y))
        (x2, y2) = (min(self.x2, other.x2), min(self.y2, other.y2))
        return Partition(x, y, x2-x, y2-y)


def sort_partitions(partitions, crops, plot):
    crops_in_part = {}
    #partial_parts_for_crop = {}
    #all_partial_parts = set()
    for (ipart, partition) in enumerate(partitions):
        for crop in crops:
            if partition.is_inside(crop):
                if partition not in crops_in_part:
                    crops_in_part[partition] = set()
                crops_in_part[partition].add(crop)
                #partial_parts_for_crop.get(crop, []).append(partition)
                #all_partial_parts.add(partition)

    grouped_partitions = []
    #step 1: group parts that contain the same set of crops
    for part in partitions:
        if part not in crops_in_part:  # Part without any crop. we create a new
                                       # group with this part only
            grouped_partitions.append([part])
            continue
        added = False
        for group in grouped_partitions:
            ref_part = group[0]
            if ref_part not in crops_in_part:
                continue
            if crops_in_part[ref_part] == crops_in_part[part]:
                group.append(part)
                added = True
                break
        if not added:
            grouped_partitions.append([part])

    #step 2: order groups
    result = grouped_partitions.pop(0)
    while grouped_partitions:
        if result[-1] not in crops_in_part:
            result.extend(grouped_partitions.pop(0))
            continue
        for (igroup, group) in enumerate(grouped_partitions):
            # Another group has common crop with the current one. we put it
            # just after
            if group[0] in crops_in_part and crops_in_part[group[0]] & crops_in_part[result[-1]]:
                result.extend(grouped_partitions.pop(igroup))
                break
        else:
            result.extend(grouped_partitions.pop(0))

    return result


def remove_partitions_overlaps(partitions):
    assert type(partitions) == set
    overlap = True
    loops = 0
    while overlap:
        overlap = None
        for partition1 in partitions:
            for partition2 in partitions:
                if partition1 == partition2:
                    continue
                if partition1.overlaps(partition2):
                    overlap = (partition1, partition2)
                    break
            if overlap:
                break
        if overlap:
            partitions.remove(partition1)
            partitions.remove(partition2)
            partitions.update(get_split_partitions(partition1, partition2))
        loops += 1
        if (loops > 200):
            raise Exception("Loops")


class Plot(Rectangle):
    def __init__(self, width, height):
        Rectangle.__init__(self, 0, 0, width, height)


def compute_partitions(plot, crops):
    all_partitions = []
    vertical = 0
    horizontal = 0
    for crop in crops:
        if crop.width == plot.width and crop.height == plot.height:
            continue
        all_partitions.append(Partition(crop.x, crop.y, crop.width, crop.height))
        if crop.width == plot.width:
            horizontal += 1
        if crop.height == plot.height:
            vertical += 1
    #all_partitions.append(Partition(0, 0, plot.width, plot.height))
    partitions = set(all_partitions)
    remove_partitions_overlaps(partitions)

    #Add partitions for empty parts of the plot
    empty_parts = set()
    allpart = Partition(0, 0, plot.width, plot.height)
    for realpart in partitions:
        empty_parts.update(get_split_partitions(realpart, allpart))
    remove_partitions_overlaps(empty_parts)
    empty_parts2 = set()
    for emptpart in empty_parts:
        remove = False
        for realpart in partitions:
            if emptpart.is_inside(realpart):
                remove = True
                continue
            assert not emptpart.overlaps(realpart)
        if not remove:
            empty_parts2.add(emptpart)
    empty_parts = empty_parts2

    partitions.update(empty_parts)

    result = []
    if vertical > horizontal:
        result = sorted(partitions, key=lambda tpl:(tpl.x, tpl.y))
    else:
        result = sorted(partitions, key=lambda tpl:(tpl.y, tpl.x))
    return result


def get_timerepr(partitions, crop, plot):
    #TODO: handle split crops
    assert type(partitions) == list

    result = []
    timeoffset = 0
    prev_repr = (-1, 0)
    for partition in partitions:
        if partition.overlaps(crop.to_partition()):
            curr_repr = (timeoffset/plot.area, partition.area/plot.area)
            if (abs(prev_repr[0] + prev_repr[1] - curr_repr[0]) < 0.001):
                curr_repr = (prev_repr[0], prev_repr[1] + curr_repr[1])
                result[-1] = curr_repr
            else:
                result.append(curr_repr)
            prev_repr = curr_repr
        timeoffset += partition.area
    assert crop.area/plot.area == sum([elem[1] for elem in result]), "%f - %s" % (crop.area/plot.area, str(result))
    return result


class Crop(Rectangle):
    def __init__(self, plot, start, end, x, y, width, height):
        Rectangle.__init__(self, x, y, width, height)
        self.plot = plot
        self.start = start
        self.end = end
        self.timerepr = [];
    def __hash__(self):
        return hash(self.__repr__())
    def __repr__(self):
        return "Crop(%s, %s, %s, %s)" % (self.x, self.y, self.width, self.height)
    def to_partition(self):
        return Partition(self.x, self.y, self.width, self.height)


def get_nb_splits(crops):
    result = 0
    for crop in crops:
        result += len(crop.timerepr) - 1
    return result


def fill_timeoffsets(plot, crops):
    partitions = compute_partitions(plot, crops)
    #partitions = sort_partitions(partitions_set, vertical=vertical>horizontal, crops=crops)
    partitions = sort_partitions(partitions, crops=crops, plot=plot)
    for crop in crops:
        crop.timerepr = get_timerepr(partitions, crop, plot)
    return partitions


def write_partitions(dwg, partitions):
    fact = 2.5
    for (ipart, part) in enumerate(partitions):
        rect_pos = ((20 + part.x * fact) + 1100, 300 + (20 + part.y * fact))
        rect_dim = (part.width * fact, part.height * fact)
        dwg.add(dwg.rect(rect_pos, rect_dim, stroke="blue", fill_opacity=0))
        text_pos = (rect_pos[0] + 2, rect_pos[1]+15)
        dwg.add(dwg.text("%i" % (ipart), insert = text_pos))


def write_space_repr(dwg, plot, crops, time):
    fact = 2.5
    xoffset = 5.5 * time
    rect_dim = (plot.width * fact, plot.height * fact)
    dwg.add(dwg.rect((20 + (xoffset * fact), 20), rect_dim, stroke="black", fill_opacity=0))
    for (icrop, crop) in enumerate(crops):
        if crop.start > time:
            continue
        if crop.end < time:
            continue
        rect_pos = ((20 + (xoffset + crop.x) * fact), (20 + crop.y * fact))
        rect_dim = (crop.width * fact, crop.height * fact)
        dwg.add(dwg.rect(rect_pos, rect_dim, stroke="blue", fill_opacity=0))
        text_pos = (rect_pos[0] + 2, rect_pos[1]+15)
        dwg.add(dwg.text("%i" % (icrop), insert = text_pos))


def write_time_repr(dwg, plot, crops):
    yoffset = 300
    timeheight = 300
    dwg.add(dwg.rect((20, yoffset), (1000, 300), stroke="black", fill_opacity=0))
    for (icrop, crop) in enumerate(crops):
        for trep in crop.timerepr:
            rect_pos = ((20 + (10 * crop.start)), yoffset + 300 * trep[0])
            rect_dim = (10 * (crop.end - crop.start), 300 * trep[1])
            dwg.add(dwg.rect(rect_pos, rect_dim, stroke="blue", fill_opacity=0))
            text_pos = (rect_pos[0] + 2, rect_pos[1]+15)
            dwg.add(dwg.text("%i" % (icrop), insert = text_pos))


def get_split_partitions(partition1, partition2):
    assert partition1.overlaps(partition2)
    if partition1.is_inside(partition2) or partition2.is_inside(partition1):
        y_points = [partition1.y, partition1.y2,
                    partition2.y, partition2.y2]
        y_points = sorted(set(y_points))
        x_points = [partition1.x, partition1.x2,
                    partition2.x, partition2.x2]
        x_points = sorted(set(x_points))
        result = set()
        for x in range(len(x_points) - 1):
            for y in range(len(y_points) - 1):
                result.add(Partition(x_points[x], y_points[y], x_points[x+1] - x_points[x], y_points[y+1] - y_points[y]))
        return result
    else:
        intersect = partition1.intersection(partition2)
        assert intersect.is_inside(partition1) and intersect.is_inside(partition2), \
               "%s - %s - %s" % (intersect, partition1, partition2)
        result = set()
        result.update(get_split_partitions(partition1, intersect))
        result.update(get_split_partitions(partition2, intersect))
        return result

    raise Exception("split case unhandled: %s - %s" % (partition1, partition2))


def main():
    test()


def write_svg(plot, crops, filename, partitions=None):
    dwg = svgwrite.Drawing(filename, size=("1500px", "1000px"))
    for i in range(5):
        write_space_repr(dwg, plot, crops, i*20)

    write_time_repr(dwg, plot, crops)
    if partitions is not None:
        write_partitions(dwg, partitions)
    text_pos = (20, 650)
    dwg.add(dwg.text("%i splits" % get_nb_splits(crops), insert = text_pos))
    dwg.save()



##############################################################################
#  tests
##############################################################################
import unittest
class TestPartitions(unittest.TestCase):
    def test01_split(self):
        self.assertEqual(get_split_partitions(Partition(0, 0, 30, 30), Partition(10, 0, 10, 30)),
                         set([Partition(0, 0, 10, 30), Partition(10, 0, 10, 30), Partition(20, 0, 10, 30)]))
        self.assertEqual(get_split_partitions(Partition(0, 0, 30, 30), Partition(0, 10, 30, 10)),
                         set([Partition(0, 0, 30, 10), Partition(0, 10, 30, 10), Partition(0, 20, 30, 10)]))
        self.assertEqual(get_split_partitions(Partition(75, 50, 25, 25), Partition(50, 50, 50, 25)),
                         set([Partition(50, 50, 25, 25), Partition(75, 50, 25, 25)]))

    def test02_inside(self):
        self.assertTrue(Partition(75, 80, 25, 10).is_inside(Partition(0, 0, 100, 90)))
        self.assertFalse(Partition(76, 80, 25, 10).is_inside(Partition(0, 0, 100, 90)))
        self.assertTrue(Rectangle(15, 10, 25, 30).is_inside(Rectangle(10, 10, 30, 30)))
        self.assertTrue(Rectangle(10, 10, 30, 30).is_inside(Rectangle(10, 10, 30, 30)))

class TestAll(unittest.TestCase):
    def test10_vertical(self):
        plot = Plot(100, 100)
        crops = [Crop(plot, 0, 100, 0, 0, 25, 100),
                 Crop(plot, 20, 100, 25, 0, 25, 100),
                 Crop(plot, 0, 30, 60, 0, 20, 100),
                 Crop(plot, 0, 50, 80, 0, 20, 100),
                 Crop(plot, 50, 100, 70, 0, 20, 100)]
        fill_timeoffsets(plot, crops)
        timeoffsets = [crop.timerepr[0][0] for crop in crops]
        write_svg(plot, crops, "test10.svg")
        self.assertEqual(timeoffsets, [0, 0.25, 0.60, 0.80, 0.70])

    def test20_vertical_plus(self):
        plot = Plot(100, 100)
        crops = [Crop(plot, 0, 100, 0, 0, 25, 100),
                 Crop(plot, 20, 100, 25, 0, 25, 100),
                 Crop(plot, 0, 30, 60, 0, 20, 50),
                 Crop(plot, 20, 70, 60, 50, 20, 50),
                 Crop(plot, 0, 50, 80, 0, 20, 100),
                 Crop(plot, 50, 100, 60, 0, 20, 100)]
        partitions = fill_timeoffsets(plot, crops)
        timeoffsets = [crop.timerepr[0][0] for crop in crops]
        write_svg(plot, crops, "test20.svg", partitions)
        self.assertEqual(timeoffsets, [0, 0.25, 0.60, 0.70, 0.80, 0.60])

    def test21_vertical_plus(self):
        plot = Plot(100, 100)
        crops = [Crop(plot, 0, 100, 0, 0, 25, 100),
                 Crop(plot, 20, 100, 25, 0, 25, 100),
                 Crop(plot, 0, 30, 60, 0, 20, 50),
                 Crop(plot, 20, 70, 70, 50, 10, 50),
                 Crop(plot, 0, 50, 80, 0, 20, 100),
                 Crop(plot, 50, 100, 60, 0, 20, 100)]
        fill_timeoffsets(plot, crops)
        timeoffsets = [crop.timerepr[0][0] for crop in crops]
        write_svg(plot, crops, "test21.svg")
        self.assertEqual(timeoffsets, [0, 0.25, 0.60, 0.75, 0.80, 0.60])

    def test25_vertical_plus(self):
        plot = Plot(100, 100)
        crops = [Crop(plot, 0, 100, 0, 0, 25, 100),
                 Crop(plot, 20, 100, 25, 0, 25, 100),
                 Crop(plot, 0, 30, 60, 0, 20, 50),
                 Crop(plot, 40, 70, 60, 50, 20, 50),
                 Crop(plot, 0, 50, 80, 0, 20, 100),
                 Crop(plot, 50, 100, 70, 0, 20, 100)]
        partitions = fill_timeoffsets(plot, crops)
        timereprs = [crop.timerepr for crop in crops]
        write_svg(plot, crops, "test25.svg", partitions)
        self.assertEqual(timereprs, [[(0.0, 0.25)], [(0.25, 0.25)],
                                     [(0.6, 0.1)], [(0.7, 0.1)],
                                     [(0.8, 0.2)],
                                     [(0.65, 0.1), (0.8, 0.1)]])
        #self.assertEqual(timereprs, [[(0.0, 0.25)], [(0.25, 0.25)],
        #                             [(0.6, 0.05), (0.7, 0.05)],
        #                             [(0.65, 0.05), (0.75, 0.05)],
        #                             [(0.8, 0.2)], [(0.7, 0.2)]])


    def test30_squares(self):
        plot = Plot(100, 100)
        crops = [Crop(plot, 0, 100, 0, 0, 25, 25),
                 Crop(plot, 0, 100, 75, 75, 25, 25),
                 Crop(plot, 40, 70, 50, 50, 25, 25),
                 Crop(plot, 30, 80, 25, 50, 25, 25),
                 Crop(plot, 30, 80, 50, 25, 25, 25),
                 Crop(plot, 60, 100, 75, 25, 25, 25),
                 Crop(plot, 70, 100, 50, 75, 25, 25),
                 Crop(plot, 70, 100, 25, 0, 25, 25),
                 Crop(plot, 70, 100, 0, 75, 25, 25),
                 Crop(plot, 80, 100, 0, 25, 25, 25)
                ]
        partitions = fill_timeoffsets(plot, crops)
        timeoffsets = [crop.timerepr[0][0] for crop in crops]
        write_svg(plot, crops, "test30.svg", partitions)
        self.assertEqual(timeoffsets, [0.0, 0.9375, 0.625, 0.5625, 0.375, 0.4375, 0.875, 0.0625, 0.75, 0.25])

    def test31_squares(self):
        plot = Plot(100, 100)
        crops = [Crop(plot, 0, 100, 0, 0, 25, 25),
                 Crop(plot, 0, 100, 75, 75, 25, 25),
                 Crop(plot, 40, 70, 50, 50, 25, 25),
                 Crop(plot, 30, 80, 25, 50, 25, 25),
                 Crop(plot, 60, 100, 75, 25, 25, 25),
                 Crop(plot, 70, 100, 50, 75, 25, 25),
                 Crop(plot, 80, 100, 0, 25, 25, 25)
                ]
        partitions = fill_timeoffsets(plot, crops)
        timeoffsets = [crop.timerepr[0][0] for crop in crops]
        write_svg(plot, crops, "test31.svg", partitions)
        self.assertEqual(timeoffsets, [0.0, 0.9375, 0.625, 0.5625, 0.4375, 0.875, 0.25])

    def test35_squares(self):
        plot = Plot(100, 100)
        crops = [Crop(plot, 0, 100, 0, 0, 25, 25),
                 Crop(plot, 0, 100, 75, 75, 25, 25),
                 Crop(plot, 0, 30, 50, 50, 50, 25),
                 Crop(plot, 40, 70, 50, 50, 25, 25),
                 Crop(plot, 70, 100, 75, 50, 25, 25),
                 Crop(plot, 60, 70, 0, 0, 100, 100)]
        fill_timeoffsets(plot, crops)
        timereprs = [crop.timerepr for crop in crops]
        write_svg(plot, crops, "test35.svg")
        self.assertEqual(timereprs, [[(0.0, 1/16)], [(1-1/16, 1/16)],
                                     [(1-3/16, 1/8)], [(1-3/16, 1/16)],
                                     [(1-2/16, 1/16)], [(0.0, 1.0)]])

    def test36_squares(self):
        plot = Plot(90, 90)
        crops = []
        for x in range(3):
            for y in range(3):
                crops.append(Crop(plot, 0, 100, 30*x, 30*y, 30, 30))
        partitions = fill_timeoffsets(plot, crops)
        timereprs = [crop.timerepr for crop in crops]
        write_svg(plot, crops, "test36.svg", partitions)
        self.assertEqual(timereprs, [[(0.0, 1/9)], [(1/3, 1/9)], [(2/3, 1/9)],
                                     [(1/9, 1/9)], [(4/9, 1/9)], [(7/9, 1/9)],
                                     [(2/9, 1/9)], [(5/9, 1/9)], [(8/9, 1/9)]])

    def test37_squares(self):
        plot = Plot(90, 90)
        crops = []
        for x in range(3):
            for y in range(3):
                crops.append(Crop(plot, 0, 50, 30*x, 30*y, 30, 30))
        crops.append(Crop(plot, 50, 100, 15, 15, 30, 30))
        partitions = fill_timeoffsets(plot, crops)
        timereprs = [crop.timerepr for crop in crops]
        write_svg(plot, crops, "test37.svg", partitions)
        self.assertEqual(timereprs, [[(0.0, 1/9)], [(1/3, 1/9)], [(2/3, 1/9)],
                                     [(1/9, 1/9)], [(4/9, 1/9)], [(7/9, 1/9)],
                                     [(2/9, 1/9)], [(5/9, 1/9)], [(8/9, 1/9)],
                                     [(3/36, 1/18), (15/36, 1/18)]])

    def test38_squares(self):
        plot = Plot(100, 100)
        crops = []
        for x in range(2):
            for y in range(2):
                crops.append(Crop(plot, 0, 50, 50*x, 50*y, 50, 50))
        crops.append(Crop(plot, 50, 100, 25, 25, 50, 50))
        partitions = fill_timeoffsets(plot, crops)
        timereprs = [crop.timerepr for crop in crops]
        write_svg(plot, crops, "test38.svg", partitions)
        #self.assertEqual(timereprs, [[(0.0, 1/9)], [(1/3, 1/9)], [(2/3, 1/9)],
        #                             [(1/9, 1/9)], [(4/9, 1/9)], [(7/9, 1/9)],
        #                             [(2/9, 1/9)], [(5/9, 1/9)], [(8/9, 1/9)],
        #                             [(0, 0)]])

    def test40_crosses(self):
        plot = Plot(100, 100)
        crops = [Crop(plot, 0, 49, 0, 0, 50, 100),
                 Crop(plot, 0, 49, 50, 0, 50, 100),
                 Crop(plot, 50, 100, 0, 50, 100, 50),
                 Crop(plot, 50, 100, 0, 0, 100, 50)]
        partitions = fill_timeoffsets(plot, crops)
        timereprs = [crop.timerepr for crop in crops]
        write_svg(plot, crops, "test40.svg", partitions)
        #self.assertEqual(timereprs, [[(0.0, 0.25), (0.5, 0.25)], [(0.25, 0.25), (0.75, 0.25)],
        #                             [(0.5, 0.5)], [(0.0, 0.5)]])
        self.assertEqual(timereprs, [[(0.0, 0.25), (0.75, 0.25)], [(0.25, 0.5)], [(0.5, 0.5)], [(0.0, 0.5)]])


def test():
    sys.argv[1:] = []
    unittest.main()


if __name__ == "__main__":
    main()
