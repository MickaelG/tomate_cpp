#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>

#include "dataset.h"


BOOST_AUTO_TEST_CASE(plots)
{
    Dataset data;
    data.add_plot(Plot("pn", "plot_nom", "une jolie planche", Rectangle(2,3,8,9)));
    //BOOST_CHECK(add(2, 2) == 4);
}

BOOST_AUTO_TEST_CASE(crops)
{
    Plant plant("to", "tomate");
    Plot plot("p1", "Carré1", "description du carré 1", Rectangle(2,3,8,9));
    bg::date start_date(2013, 3, 3);
    bg::date end_date(2013, 3, 23);
    Crop crop(start_date, end_date, plant, "", plot);
    BOOST_CHECK(crop.get_plant().get_name() == "tomate");
    plant.set_name("Concombre");
    BOOST_CHECK(crop.get_plant().get_name() == "Concombre");
}

#include <fstream>
#include <sstream>
string slurp(ifstream& in) {
    stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

BOOST_AUTO_TEST_CASE(xml)
{
    Dataset dataset;
    xml_read_data("testfiles/data.sfg", dataset);
    xml_write_data("data_out.sfg", dataset);
    ifstream ifs1("testfiles/data.sfg");
    ifstream ifs2("data_out.sfg");
    string str1;
    string str2;
    while(getline(ifs1, str1) && getline(ifs2, str2)) {
        BOOST_REQUIRE_EQUAL(str1, str2);
    }
    BOOST_CHECK(!(ifs1 && ifs2));
}
