#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>

#include "dataset.h"


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
    xml_read_data("../testfiles/data.sfg", dataset);
    xml_write_data("data_out.sfg", dataset);
    ifstream ifs1("../testfiles/data.sfg");
    ifstream ifs2("data_out.sfg");
    string str1;
    string str2;
    while(getline(ifs1, str1) && getline(ifs2, str2)) {
        BOOST_REQUIRE_EQUAL(str1, str2);
    }
    BOOST_CHECK(!(ifs1 && ifs2));
}


BOOST_AUTO_TEST_CASE(is_active)
{
    Plant plant("pl", "plant_name");
    Plot plot("pn", "plot_name");
    
    //Crop with only a start date
    Crop crop1(bg::date(2012, 10, 02), bg::date(), plant, "", plot);
    BOOST_CHECK(crop1.is_active_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(crop1.is_active_at_date(bg::date(bg::day_clock::local_day())));
    BOOST_CHECK(!crop1.is_active_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(!crop1.is_planned_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop1.is_planned_at_date(bg::date(2012, 10, 01)));
    
    //Crop with start date and end date
    Crop crop2(bg::date(2012, 10, 02), bg::date(2012, 11, 22), plant, "", plot);
    BOOST_CHECK(crop2.is_active_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop2.is_active_at_date(bg::date(bg::day_clock::local_day())));
    BOOST_CHECK(!crop2.is_active_at_date(bg::date(2012, 11, 23)));
    BOOST_CHECK(!crop2.is_active_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(!crop2.is_planned_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop2.is_planned_at_date(bg::date(2012, 10, 01)));
    
    //Crop with only planned start and planned end
    Crop crop3(bg::date(), bg::date(), bg::date(2012, 10, 02), bg::date(2012, 11, 22), plant, "", plot);
    BOOST_CHECK(!crop3.is_active_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop3.is_active_at_date(bg::date(bg::day_clock::local_day())));
    BOOST_CHECK(!crop3.is_active_at_date(bg::date(2012, 11, 23)));
    BOOST_CHECK(!crop3.is_active_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(crop3.is_planned_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop3.is_planned_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(crop3.is_planned_at_date(bg::date(2012, 11, 21)));
    BOOST_CHECK(!crop3.is_planned_at_date(bg::date(2012, 11, 23)));
    
    //Crop with all dates set
    Crop crop4(bg::date(2012, 10, 15), bg::date(2012, 11, 28), bg::date(2012, 10, 02), bg::date(2012, 11, 22), plant, "", plot);
    BOOST_CHECK(!crop4.is_active_at_date(bg::date(2012, 10, 14)));
    BOOST_CHECK(!crop4.is_active_at_date(bg::date(2012, 11, 29)));
    BOOST_CHECK(crop4.is_active_at_date(bg::date(2012, 10, 16)));
    BOOST_CHECK(crop4.is_active_at_date(bg::date(2012, 11, 27)));
    BOOST_CHECK(crop4.is_planned_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop4.is_planned_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(crop4.is_planned_at_date(bg::date(2012, 11, 21)));
    BOOST_CHECK(!crop4.is_planned_at_date(bg::date(2012, 11, 23)));
    
}

BOOST_AUTO_TEST_CASE(find_crop)
{
    Dataset data;
    Plot& plot = data.add_plot(Plot("pn", "plot_nom", "une jolie planche", Rectangle(2,3,8,9)));
    Plot& plot2 = data.add_plot(Plot("p2", "plot_nom2", "une très jolie planche", Rectangle(2,3,8,9)));
    Plant& plant = data.add_plant(Plant("pl", "plant_name"));
    data.add_crop(Crop(bg::date(2012, 8, 15), bg::date(2012, 9, 10), plant, "", plot, "first crop"));
    data.add_crop(Crop(bg::date(2012, 10, 02), bg::date(2012, 11, 10), plant, "", plot, "second crop"));
    data.add_crop(Crop(bg::date(), bg::date(), bg::date(2012, 12, 1), bg::date(2013, 2, 9), plant, "", plot, "third, planned crop"));
    data.add_crop(Crop(bg::date(2012, 10, 02), bg::date(2012, 12, 10), plant, "", plot2, "other crop"));
    
    Crop& crop0 = data.get_crops().find_crop(plot, bg::date(2012, 11, 1));
    BOOST_CHECK_EQUAL(crop0.get_note(), "second crop");
    BOOST_CHECK( crop0 );
    Crop& crop1 = data.get_crops().find_crop(plot, bg::date(2012, 9, 1));
    BOOST_CHECK_EQUAL(crop1.get_note(), "first crop");
    BOOST_CHECK( crop1 );
    Crop& crop2 = data.get_crops().find_crop(plot, bg::date(2012, 11, 21));
    BOOST_CHECK( !crop2 );
    BOOST_CHECK_EQUAL(crop2.get_plant().get_name(), "");
    Crop& crop3 = data.get_crops().find_crop(plot2, bg::date(2012, 11, 21));
    BOOST_CHECK( crop3 );
    BOOST_CHECK_EQUAL(crop3.get_note(), "other crop");
    Crop crop4 = data.get_crops().find_crop(plot, bg::date(2012, 12, 21));
    BOOST_CHECK( crop4 );
    BOOST_CHECK_EQUAL(crop4.get_note(), "third, planned crop");
}
    