#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>

#include "dataset.h"
#include "xml.h"


BOOST_AUTO_TEST_CASE(crops)
{
    Plant plant("to", "tomate");
    Plot plot("p1", "Carré1", "description du carré 1", Rectangle(2,3,8,9));
    bg::date start_date(2013, 3, 3);
    bg::date end_date(2013, 3, 23);
    Crop crop(start_date, end_date, &plant, "", &plot);
    BOOST_CHECK(crop.get_plant().get_name() == "tomate");
    plant.set_name("Concombre");
    BOOST_CHECK(crop.get_plant().get_name() == "Concombre");
    Plant plant2("ra", "radis");
    crop.set_plant(plant2);
    BOOST_CHECK(crop.get_plant().get_name() == "radis");
    Plot plot2("p2", "Carré2");
    BOOST_CHECK(crop.get_plot().get_name() == "Carré1");
    crop.set_plot(plot2);
    BOOST_CHECK(crop.get_plot().get_name() == "Carré2");
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
    Crop crop1(bg::date(2012, 10, 02), bg::date(), &plant, "", &plot);
    BOOST_CHECK(crop1.is_active_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(crop1.is_active_at_date(bg::date(bg::day_clock::local_day())));
    BOOST_CHECK(!crop1.is_active_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(!crop1.is_planned_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop1.is_planned_at_date(bg::date(2012, 10, 01)));
    
    //Crop with start date and end date
    Crop crop2(bg::date(2012, 10, 02), bg::date(2012, 11, 22), &plant, "", &plot);
    BOOST_CHECK(crop2.is_active_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop2.is_active_at_date(bg::date(bg::day_clock::local_day())));
    BOOST_CHECK(!crop2.is_active_at_date(bg::date(2012, 11, 23)));
    BOOST_CHECK(!crop2.is_active_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(!crop2.is_planned_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop2.is_planned_at_date(bg::date(2012, 10, 01)));
    
    //Crop with only planned start and planned end
    Crop crop3(bg::date(), bg::date(), bg::date(2012, 10, 02), bg::date(2012, 11, 22), &plant, "", &plot);
    BOOST_CHECK(!crop3.is_active_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop3.is_active_at_date(bg::date(bg::day_clock::local_day())));
    BOOST_CHECK(!crop3.is_active_at_date(bg::date(2012, 11, 23)));
    BOOST_CHECK(!crop3.is_active_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(crop3.is_planned_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop3.is_planned_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(crop3.is_planned_at_date(bg::date(2012, 11, 21)));
    BOOST_CHECK(!crop3.is_planned_at_date(bg::date(2012, 11, 23)));
    
    //Crop with all dates set
    Crop crop4(bg::date(2012, 10, 15), bg::date(2012, 11, 28), bg::date(2012, 10, 02), bg::date(2012, 11, 22), &plant, "", &plot);
    BOOST_CHECK(!crop4.is_active_at_date(bg::date(2012, 10, 14)));
    BOOST_CHECK(!crop4.is_active_at_date(bg::date(2012, 11, 29)));
    BOOST_CHECK(crop4.is_active_at_date(bg::date(2012, 10, 16)));
    BOOST_CHECK(crop4.is_active_at_date(bg::date(2012, 11, 27)));
    BOOST_CHECK(crop4.is_planned_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop4.is_planned_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(crop4.is_planned_at_date(bg::date(2012, 11, 21)));
    BOOST_CHECK(!crop4.is_planned_at_date(bg::date(2012, 11, 23)));

    //Crop with star date and planned end
    Crop crop5(bg::date(2012, 10, 15), bg::date(), bg::date(), bg::date(2012, 11, 22), &plant, "", &plot);
    BOOST_CHECK(!crop5.is_active_at_date(bg::date(2012, 10, 14)));
    BOOST_CHECK(crop5.is_active_at_date(bg::date(2012, 10, 16)));
    BOOST_CHECK(crop5.is_active_at_date(bg::date(2012, 11, 27)));
    BOOST_CHECK(!crop5.is_planned_at_date(bg::date(2012, 10, 14)));
    BOOST_CHECK(crop5.is_planned_at_date(bg::date(2012, 10, 16)));
    BOOST_CHECK(crop5.is_planned_at_date(bg::date(2012, 11, 21)));
    BOOST_CHECK(!crop5.is_planned_at_date(bg::date(2012, 11, 23)));

}

BOOST_AUTO_TEST_CASE(find_crop)
{
    Dataset data;
    Plot& plot = data.get_plots().add_plot("pn", "plot_nom", "une jolie planche", 2,3,8,9);
    Plot& plot2 = data.get_plots().add_plot("p2", "plot_nom2", "une très jolie planche", 2,3,8,9);
    Plant& plant = data.add_plant(Plant("pl", "plant_name"));
    data.add_crop(Crop(bg::date(2012, 8, 15), bg::date(2012, 9, 10), &plant, "", &plot, "first crop"));
    data.add_crop(Crop(bg::date(2012, 10, 02), bg::date(2012, 11, 10), &plant, "", &plot, "second crop"));
    data.add_crop(Crop(bg::date(), bg::date(), bg::date(2012, 12, 1), bg::date(2013, 2, 9), &plant, "", &plot, "third, planned crop"));
    data.add_crop(Crop(bg::date(2012, 10, 02), bg::date(2012, 12, 10), &plant, "", &plot2, "other crop"));
    
    Crop* p_crop0 = data.get_crops().find_pcrop(plot, bg::date(2012, 11, 1));
    BOOST_CHECK_EQUAL(p_crop0->get_note(), "second crop");
    BOOST_CHECK( p_crop0 );
    Crop* p_crop1 = data.get_crops().find_pcrop(plot, bg::date(2012, 9, 1));
    BOOST_CHECK_EQUAL(p_crop1->get_note(), "first crop");
    BOOST_CHECK( p_crop1 );
    const Crop* p_crop2 = data.get_crops().find_pcrop(plot, bg::date(2012, 11, 21));
    BOOST_CHECK( !p_crop2 );
    Crop* p_crop3 = data.get_crops().find_pcrop(plot2, bg::date(2012, 11, 21));
    BOOST_CHECK( p_crop3 );
    BOOST_CHECK_EQUAL(p_crop3->get_note(), "other crop");
    Crop* p_crop4 = data.get_crops().find_pcrop(plot, bg::date(2012, 12, 21));
    BOOST_CHECK( p_crop4 );
    BOOST_CHECK_EQUAL(p_crop4->get_note(), "third, planned crop");

    //delete_plot
    Crop* p_crop5 = data.get_crops().find_pcrop(plot, bg::date(2012, 11, 1));
    BOOST_CHECK( p_crop5 );
    data.get_crops().delete_crop(*p_crop5);
    Crop* p_crop6 = data.get_crops().find_pcrop(plot, bg::date(2012, 11, 1));
    BOOST_CHECK( !p_crop6 );
    Crop* p_crop7 = data.get_crops().find_pcrop(plot2, bg::date(2012, 11, 21));
    BOOST_CHECK( p_crop7 );
    BOOST_CHECK_EQUAL(p_crop7->get_note(), "other crop");
}

BOOST_AUTO_TEST_CASE(plots)
{
    Dataset data;
    Plot& plot = data.get_plots().add_plot("te", "test");
    Plot& plt2 = data.get_plots().add_plot("te2", "test2");

    BOOST_CHECK_EQUAL(data.get_plots().size(), 2);
    BOOST_CHECK_EQUAL(data.get_plots().index(0).get_key(), "te");
    BOOST_CHECK_EQUAL(data.get_plots().index(1).get_key(), "te2");
    vector<string> keys;
    for(Plot plot: data.get_plots())
    {
        keys.push_back(plot.get_key());
    }
    BOOST_CHECK_EQUAL(keys.size(), 2);

    data.get_plots().delete_plot("te");

    BOOST_CHECK_EQUAL(data.get_plots().size(), 1);
    BOOST_CHECK_EQUAL(data.get_plots().index(0).get_key(), "te2");
    keys.clear();
    for(Plot plot: data.get_plots())
    {
        keys.push_back(plot.get_key());
    }
    BOOST_CHECK_EQUAL(keys.size(), 1);

    data.get_plots().delete_plot("te2");

    BOOST_CHECK_EQUAL(data.get_plots().size(), 0);
    keys.clear();
    for(Plot plot: data.get_plots())
    {
        keys.push_back(plot.get_key());
    }
    BOOST_CHECK_EQUAL(keys.size(), 0);
}

BOOST_AUTO_TEST_CASE(plants)
{
    Dataset data;
    Plant& plant = data.get_plants().add_plant("pl1", "plant1");
    Plot plot("p1", "Carré1", "description du carré 1", Rectangle(2,3,8,9));
    bg::date start_date(2013, 3, 3);
    bg::date end_date(2013, 3, 23);

    BOOST_CHECK_EQUAL(data.get_plants().is_used(plant), false);
    data.add_crop(Crop(start_date, end_date, &plant, "", &plot));
    BOOST_CHECK_EQUAL(data.get_plants().is_used(plant), true);
}

