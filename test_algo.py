#!/usr/bin/python3

"""
Prototype script for spaceview to timeview conversion
"""

import sys
import svgwrite

class Partition:
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
    def __getitem__(self, idx):
        if idx == 0:
            return self.x
        elif idx == 1:
            return self.y
        elif idx == 2:
            return self.width
        elif idx == 3:
            return self.height
    @property
    def x2(self):
        return self.x + self.width
    @property
    def y2(self):
        return self.y + self.height
    def get_area(self):
        return self.width * self.height
    def __lt__(self, other):
        return self.tuple() < other.tuple()
    def __eq__(self, other):
        return self.tuple() == other.tuple()
    def tuple(self):
        return (self.x, self.y, self.width, self.height)
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
    def is_inside(self, other):
        for dim in (0, 1):
            if self[0+dim] < other[0+dim]:
                return False
            if self[0+dim] + self[2+dim] > other[0+dim] + other[2+dim]:
                return False
        return True
    def intersection(self, other):
        assert self.overlaps(other)
        (x, y) = (max(self.x, other.x), max(self.y, other.y))
        (x2, y2) = (min(self.x2, other.x2), min(self.y2, other.y2))
        return Partition(x, y, x2-x, y2-y)

def sort_partitions(partitions):
    return sorted(partitions)

class Plot:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.partitions = None

    def compute_partitions(self, crops):
        all_partitions = []
        for crop in crops:
            all_partitions.append(Partition(crop.x, crop.y, crop.width, crop.height))
        all_partitions.append(Partition(0, 0, self.width, self.height))
        partitions = set(all_partitions)
        overlap = True
        loops = 0;
        while overlap:
            #import pdb; pdb.set_trace()
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
            if (loops > 20):
                raise Exception("Loops")
        self.partitions = sort_partitions(partitions)
        return self.partitions

def get_timeoffset(partitions, crop):
    #TODO: handle split crops
    assert type(partitions) == list

    timeoffset = 0
    for partition in partitions:
        if partition.overlaps(crop.to_partition()):
            break
        timeoffset += partition.get_area()
    return timeoffset


class Crop:
    def __init__(self, plot, start, end, x, y, width, height):
        self.plot = plot
        self.start = start
        self.end = end
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.timeoffset = 0;
        self.timeheight = (self.width * self.height) / (plot.width * plot.height)
    def to_partition(self):
        return Partition(self.x, self.y, self.width, self.height)


def fill_timeoffsets(plot, crops):
    partitions = plot.compute_partitions(crops)
    for crop in crops:
        crop.timeoffset = get_timeoffset(partitions, crop) / (plot.width * plot.height)

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
        rect_pos = ((20 + (10 * crop.start)), yoffset + 300 * crop.timeoffset)
        rect_dim = (10 * (crop.end - crop.start), 300 * crop.timeheight)
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
    if len(sys.argv) > 1 and sys.argv[1] == "test":
        test()
    else:
        run()

def write_svg(plot, crops, filename):
    dwg = svgwrite.Drawing(filename, size=("1500px", "1000px"))
    for i in range(5):
        write_space_repr(dwg, plot, crops, i*20)

    write_time_repr(dwg, plot, crops)
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

class TestAll(unittest.TestCase):
    def test10_vertical(self):
        plot = Plot(100, 100)
        crops = [Crop(plot, 0, 100, 0, 0, 25, 100),
                 Crop(plot, 20, 100, 25, 0, 25, 100),
                 Crop(plot, 0, 30, 60, 0, 20, 100),
                 Crop(plot, 0, 50, 80, 0, 20, 100),
                 Crop(plot, 50, 100, 70, 0, 20, 100)]
        fill_timeoffsets(plot, crops)
        timeoffsets = [crop.timeoffset for crop in crops]
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
        fill_timeoffsets(plot, crops)
        timeoffsets = [crop.timeoffset for crop in crops]
        write_svg(plot, crops, "test20.svg")
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
        timeoffsets = [crop.timeoffset for crop in crops]
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
        fill_timeoffsets(plot, crops)
        timeoffsets = [crop.timeoffset for crop in crops]
        write_svg(plot, crops, "test25.svg")
        self.assertEqual(timeoffsets, [0, 0.25, 0.60, 0.70, 0.80, 0.70])


    def test30_squares(self):
        plot = Plot(100, 100)
        crops = [Crop(plot, 0, 100, 0, 0, 25, 25),
                 Crop(plot, 0, 100, 75, 75, 25, 25),
                 Crop(plot, 0, 30, 50, 50, 50, 25),
                 Crop(plot, 40, 70, 50, 50, 25, 25),
                 Crop(plot, 70, 100, 75, 50, 25, 25),
                 Crop(plot, 60, 70, 0, 0, 100, 100)]
        fill_timeoffsets(plot, crops)
        timeoffsets = [crop.timeoffset for crop in crops]
        write_svg(plot, crops, "test30.svg")
        self.assertEqual(timeoffsets, [0, 0.9375, 0.625, 0.625, 0.6875, 0])

def test():
    sys.argv[1:] = []
    unittest.main()


if __name__ == "__main__":
    main()
