#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>

#include "dataset.h"

#include "gui_utils.h"

BOOST_AUTO_TEST_CASE(date)
{
    BOOST_CHECK(QDate::currentDate() == toQDate(bg::day_clock::local_day()));
    BOOST_CHECK(fromQDate(QDate::currentDate()) == bg::day_clock::local_day());
    
    BOOST_CHECK(QDate() == toQDate(bg::date()));
    BOOST_CHECK(fromQDate(QDate()) == bg::date());
}
