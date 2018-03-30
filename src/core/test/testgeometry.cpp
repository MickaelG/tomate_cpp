
#include <boost/test/unit_test.hpp>

#include "geometry.h"

#include <memory>
using std::unique_ptr;


BOOST_AUTO_TEST_CASE(crop_shape)
{
    Rectangle plot_rect(50, 20, 100, 120);

    unique_ptr< Rectangle > crop_rect(new Rectangle(10, 11, 20, 30));
    crop_rect->fit_in_other(plot_rect);
    BOOST_CHECK_EQUAL(crop_rect->get_width(), 10);
    BOOST_CHECK_EQUAL(crop_rect->get_height(), 21);
    BOOST_CHECK_EQUAL(crop_rect->get_min_x(), 50);
    BOOST_CHECK_EQUAL(crop_rect->get_min_y(), 20);

    crop_rect.reset(new Rectangle(55, 65, 70, 70));
    crop_rect->fit_in_other(plot_rect);
    BOOST_CHECK_EQUAL(crop_rect->get_width(), 70);
    BOOST_CHECK_EQUAL(crop_rect->get_height(), 70);
    BOOST_CHECK_EQUAL(crop_rect->get_min_x(), 55);
    BOOST_CHECK_EQUAL(crop_rect->get_min_y(), 65);

    crop_rect.reset(new Rectangle(150, 150, 70, 70));
    crop_rect->fit_in_other(plot_rect);
    BOOST_CHECK_EQUAL(crop_rect->get_width(), 10);
    BOOST_CHECK_EQUAL(crop_rect->get_height(), 10);
    BOOST_CHECK_EQUAL(crop_rect->get_min_x(), 140);
    BOOST_CHECK_EQUAL(crop_rect->get_min_y(), 130);

}

BOOST_AUTO_TEST_CASE(geometry)
{
    unique_ptr< Shape > Rect1(new Rectangle(0, 0, 10, 10));
    unique_ptr< Shape > Rect2(new Rectangle(5, 5, 10, 10));
    BOOST_CHECK_EQUAL(Rect1->get_area(), 100);
    BOOST_CHECK_EQUAL(Rect2->get_area(), 100);
    Rect1->fit_in_other(*Rect2);
    BOOST_CHECK_EQUAL(Rect1->get_min_x(), 5);
    BOOST_CHECK_EQUAL(Rect1->get_min_y(), 5);
    BOOST_CHECK_EQUAL(Rect1->get_height(), 5);
    BOOST_CHECK_EQUAL(Rect1->get_width(), 5);
    BOOST_CHECK_EQUAL(Rect1->get_area(), 25);
}

BOOST_AUTO_TEST_CASE(rectangle)
{
    Rectangle rect(0, 0, 120, 120);
    BOOST_CHECK_EQUAL(rect.get_min_x(), 0);
    BOOST_CHECK_EQUAL(rect.get_max_x(), 120);
    BOOST_CHECK_EQUAL(rect.get_min_y(), 0);
    BOOST_CHECK_EQUAL(rect.get_max_y(), 120);
}
