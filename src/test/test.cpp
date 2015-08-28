#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>

#include "dataset.h"
#include "plot.h"
#include "plant.h"
#include "xml.h"

BOOST_AUTO_TEST_CASE(crops)
{
    Plant plant("to", "tomate");
    Plot plot("p1", "Carré1", "description du carré 1", new Rectangle(8,9,2,3));
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
    xml_read_data("../testfiles/data_v0.2.sfg", dataset);
    BOOST_CHECK(dataset.get_plots().find("1")->get_name() == "Plot1");
    xml_write_data("data_out.sfg", dataset);
    ifstream ifs1("../testfiles/data_v0.2.sfg");
    ifstream ifs2("data_out.sfg");
    string str1;
    string str2;
    while(getline(ifs1, str1) && getline(ifs2, str2)) {
        BOOST_REQUIRE_EQUAL(str1, str2);
    }
    BOOST_CHECK(!(ifs1 && ifs2));
}

BOOST_AUTO_TEST_CASE(xml_conv_from_01)
{
    Dataset dataset;
    xml_read_data("../testfiles/data_v0.1.sfg", dataset);
    BOOST_CHECK(dataset.get_plots().find("1")->get_name() == "Plot1");
    xml_write_data("data_out.sfg", dataset);
    ifstream ifs1("../testfiles/data_v0.1.out_v0.2.sfg");
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
    BOOST_CHECK(!crop1.is_in_year_started_by(bg::date(2011, 01, 01)));
    BOOST_CHECK(crop1.is_in_year_started_by(bg::date(2012, 01, 01)));
    //BOOST_CHECK(!crop1.is_in_year_started_by(bg::date(2013, 01, 01)));

    //Crop with start date and end date
    Crop crop2(bg::date(2012, 10, 02), bg::date(2012, 11, 22), &plant, "", &plot);
    BOOST_CHECK(crop2.is_active_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop2.is_active_at_date(bg::date(bg::day_clock::local_day())));
    BOOST_CHECK(!crop2.is_active_at_date(bg::date(2012, 11, 23)));
    BOOST_CHECK(!crop2.is_active_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(!crop2.is_planned_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop2.is_planned_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(!crop2.is_in_year_started_by(bg::date(2011, 01, 01)));
    BOOST_CHECK(crop2.is_in_year_started_by(bg::date(2012, 01, 01)));
    BOOST_CHECK(!crop2.is_in_year_started_by(bg::date(2013, 01, 01)));

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
    BOOST_CHECK(!crop3.is_in_year_started_by(bg::date(2011, 01, 01)));
    BOOST_CHECK(crop3.is_in_year_started_by(bg::date(2012, 01, 01)));
    BOOST_CHECK(!crop3.is_in_year_started_by(bg::date(2013, 01, 01)));

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
    BOOST_CHECK(!crop4.is_in_year_started_by(bg::date(2011, 01, 01)));
    BOOST_CHECK(crop4.is_in_year_started_by(bg::date(2012, 01, 01)));
    BOOST_CHECK(!crop4.is_in_year_started_by(bg::date(2013, 01, 01)));

    //Crop with star date and planned end
    Crop crop5(bg::date(2012, 10, 15), bg::date(), bg::date(), bg::date(2012, 11, 22), &plant, "", &plot);
    BOOST_CHECK(!crop5.is_active_at_date(bg::date(2012, 10, 14)));
    BOOST_CHECK(crop5.is_active_at_date(bg::date(2012, 10, 16)));
    BOOST_CHECK(crop5.is_active_at_date(bg::date(2012, 11, 27)));
    BOOST_CHECK(!crop5.is_planned_at_date(bg::date(2012, 10, 14)));
    BOOST_CHECK(crop5.is_planned_at_date(bg::date(2012, 10, 16)));
    BOOST_CHECK(crop5.is_planned_at_date(bg::date(2012, 11, 21)));
    BOOST_CHECK(!crop5.is_planned_at_date(bg::date(2012, 11, 23)));
    BOOST_CHECK(!crop5.is_in_year_started_by(bg::date(2011, 01, 01)));
    BOOST_CHECK(crop5.is_in_year_started_by(bg::date(2012, 01, 01)));
    //BOOST_CHECK(!crop5.is_in_year_started_by(bg::date(2013, 01, 01)));

    //Crop with star date, end date and planned start
    Crop crop6(bg::date(2012, 10, 15), bg::date(2012, 11, 01), bg::date(2012, 10, 1), bg::date(), &plant, "", &plot);
    BOOST_CHECK(!crop6.is_active_at_date(bg::date(2012, 10, 14)));
    BOOST_CHECK(crop6.is_active_at_date(bg::date(2012, 10, 16)));
    BOOST_CHECK(crop6.is_active_at_date(bg::date(2012, 10, 30)));
    BOOST_CHECK(!crop6.is_planned_at_date(bg::date(2012, 9, 30)));
    BOOST_CHECK(crop6.is_planned_at_date(bg::date(2012, 10, 2)));
    BOOST_CHECK(!crop6.is_in_year_started_by(bg::date(2011, 01, 01)));
    BOOST_CHECK(crop6.is_in_year_started_by(bg::date(2012, 01, 01)));
    BOOST_CHECK(!crop6.is_in_year_started_by(bg::date(2013, 01, 01)));

}

BOOST_AUTO_TEST_CASE(crop_shape)
{
    Dataset data;
    Plot& plot = data.get_plots().add("pn", "plot_nom", "une jolie planche", 2,3,8,9);
    Plant& plant = data.get_plants().add("pl", "plant_name");
    Crop crop(bg::date(2012, 8, 15), bg::date(2012, 9, 10), &plant, "", &plot, "first crop");
    //BOOST_CHECK_EQUAL(crop.get_shape()->get_width(), -1);
    Rectangle *rect = new Rectangle(50, 20, 100, 120);
    plot.set_shape(rect);
    //BOOST_CHECK_EQUAL(crop.get_shape()->get_width(), 100);

    crop.set_shape(new Rectangle(10, 11, 20, 30));
    BOOST_CHECK_EQUAL(crop.get_shape()->get_width(), 20);
    BOOST_CHECK_EQUAL(crop.get_shape()->get_height(), 30);
    BOOST_CHECK_EQUAL(crop.get_shape()->get_min_x(), 10);
    BOOST_CHECK_EQUAL(crop.get_shape()->get_min_y(), 11);

    crop.set_shape(new Rectangle(55, 65, 70, 70));
    //Crops are limited to plot boundary
    BOOST_CHECK_EQUAL(crop.get_shape()->get_width(), 45);
    BOOST_CHECK_EQUAL(crop.get_shape()->get_height(), 55);
    BOOST_CHECK_EQUAL(crop.get_shape()->get_min_x(), 55);
    BOOST_CHECK_EQUAL(crop.get_shape()->get_min_y(), 65);

    crop.set_shape(new Rectangle(150, 150, 70, 70));
    //Crops are limited to plot boundary
    BOOST_CHECK_EQUAL(crop.get_shape()->get_width(), 10);
    BOOST_CHECK_EQUAL(crop.get_shape()->get_height(), 10);
    BOOST_CHECK_EQUAL(crop.get_shape()->get_min_x(), 90);
    BOOST_CHECK_EQUAL(crop.get_shape()->get_min_y(), 110);

}

BOOST_AUTO_TEST_CASE(find_crop)
{
    Dataset data;
    Plot& plot = data.get_plots().add("pn", "plot_nom", "une jolie planche", 2,3,8,9);
    Plot& plot2 = data.get_plots().add("p2", "plot_nom2", "une très jolie planche", 2,3,8,9);
    Plant& plant = data.get_plants().add("pl", "plant_name");
    data.get_crops().add(bg::date(2012, 8, 15), bg::date(2012, 9, 10), bg::date(), bg::date(), &plant, "", &plot, "first crop");
    data.get_crops().add(bg::date(2012, 10, 02), bg::date(2012, 11, 10), bg::date(), bg::date(), &plant, "", &plot, "second crop");
    data.get_crops().add(bg::date(), bg::date(), bg::date(2012, 12, 1), bg::date(2013, 2, 9), &plant, "", &plot, "third, planned crop");
    data.get_crops().add(bg::date(2012, 10, 02), bg::date(2012, 12, 10), bg::date(), bg::date(), &plant, "", &plot2, "other crop");

    BOOST_CHECK_EQUAL(data.get_crops().begin()->get_plot().get_key(), "pn");
    
    /*
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
    */
}

BOOST_AUTO_TEST_CASE(plots)
{
    Dataset data;
    Plot& plot = data.get_plots().add("te", "test");
    Plot& plt2 = data.get_plots().add("te2", "test2");

    BOOST_CHECK_EQUAL(data.get_plots().size(), 2);
    auto plot_it = data.get_plots().begin();
    BOOST_CHECK_EQUAL(plot_it->get_key(), "te");
    BOOST_CHECK_EQUAL((++plot_it)->get_key(), "te2");
    vector<string> keys;
    for(Plot plot: data.get_plots())
    {
        keys.push_back(plot.get_key());
    }
    BOOST_CHECK_EQUAL(keys.size(), 2);

    data.get_plots().remove("te");

    BOOST_CHECK_EQUAL(data.get_plots().size(), 1);
    BOOST_CHECK_EQUAL(data.get_plots().begin()->get_key(), "te2");
    keys.clear();
    for(Plot plot: data.get_plots())
    {
        keys.push_back(plot.get_key());
    }
    BOOST_CHECK_EQUAL(keys.size(), 1);

    data.get_plots().remove("te2");

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
    Plant& plant = data.get_plants().add("pl1", "plant1");
    Plot plot("p1", "Carré1", "description du carré 1", new Rectangle(8,9,2,3));
    bg::date start_date(2013, 3, 3);
    bg::date end_date(2013, 3, 23);

    BOOST_CHECK_EQUAL(data.get_plants().is_used(plant), false);
    data.get_crops().add(start_date, end_date, bg::date(), bg::date(), &plant, "", &plot);
    BOOST_CHECK_EQUAL(data.get_plants().is_used(plant), true);
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const std::set<T>& set)
{
    stream << "{";
    bool first = true;
    for (const T& item : set)
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

//hack: http://stackoverflow.com/questions/10976130/boost-check-equal-with-pairint-int-and-custom-operator
namespace std
{
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

    template <typename T1, typename T2>
    std::ostream& operator<<(std::ostream& stream, const std::pair<T1,T2>& pair)
    {
        stream << "(" << pair.first << ", " << pair.second << ")";
        return stream;
    }
}

#include "geometry.h"
BOOST_AUTO_TEST_CASE(geometry_get_split)
{
    set<Rectangle> result = compute_non_overlapping_rects(Rectangle(0, 0, 30, 30), Rectangle(10, 0, 10, 30));
    set<Rectangle> ref_result;
    ref_result.insert(Rectangle(0, 0, 10, 30));
    ref_result.insert(Rectangle(10, 0, 10, 30));
    ref_result.insert(Rectangle(20, 0, 10, 30));
    BOOST_CHECK_EQUAL(result, ref_result);

    result = compute_non_overlapping_rects(Rectangle(0, 0, 30, 30), Rectangle(0, 10, 30, 10));
    ref_result.clear();
    ref_result.insert(Rectangle(0, 0, 30, 10));
    ref_result.insert(Rectangle(0, 10, 30, 10));
    ref_result.insert(Rectangle(0, 20, 30, 10));
    BOOST_CHECK_EQUAL(result, ref_result);

    result = compute_non_overlapping_rects(Rectangle(75, 50, 25, 25), Rectangle(50, 50, 50, 25));
    ref_result.clear();
    ref_result.insert(Rectangle(50, 50, 25, 25));
    ref_result.insert(Rectangle(75, 50, 25, 25));
    BOOST_CHECK_EQUAL(result, ref_result);
}

BOOST_AUTO_TEST_CASE(geometry_is_inside)
{
    BOOST_CHECK( Rectangle(75, 80, 25, 10).is_inside(Rectangle(0, 0, 100, 90)));
    BOOST_CHECK(!Rectangle(76, 80, 25, 10).is_inside(Rectangle(0, 0, 100, 90)));
    BOOST_CHECK( Rectangle(15, 10, 25, 30).is_inside(Rectangle(10, 10, 30, 30)));
    BOOST_CHECK( Rectangle(10, 10, 30, 30).is_inside(Rectangle(10, 10, 30, 30)));
}

BOOST_AUTO_TEST_CASE(geometry_overlaps)
{
    BOOST_CHECK( Rectangle(0, 0, 65, 100).overlaps(Rectangle(0, 0, 60, 100)));
    BOOST_CHECK( Rectangle(0, 0, 25, 100).overlaps(Rectangle(0, 0, 60, 100)));
    BOOST_CHECK(!Rectangle(0, 0, 25, 100).overlaps(Rectangle(30, 0, 30, 100)));
    BOOST_CHECK( Rectangle(60, 0, 20, 100).overlaps(Rectangle(70, 0, 20, 100)));
}

#include "partitionset.h"
BOOST_AUTO_TEST_CASE(partitions_sort)
{
    list<Rectangle> lrect;
    lrect.push_back(Rectangle(0, 0, 25, 25));
    lrect.push_back(Rectangle(0, 25, 25, 25));
    lrect.push_back(Rectangle(25, 25, 25, 25));
    lrect.push_back(Rectangle(25, 0, 25, 25));

    lrect.sort(is_before_hori);
    list<Rectangle> lrecth;
    lrecth.push_back(Rectangle(0, 0, 25, 25));
    lrecth.push_back(Rectangle(25, 0, 25, 25));
    lrecth.push_back(Rectangle(0, 25, 25, 25));
    lrecth.push_back(Rectangle(25, 25, 25, 25));
    BOOST_CHECK_EQUAL(lrect, lrecth);

    lrect.sort(is_before_vert);
    list<Rectangle> lrectv;
    lrectv.push_back(Rectangle(0, 0, 25, 25));
    lrectv.push_back(Rectangle(0, 25, 25, 25));
    lrectv.push_back(Rectangle(25, 0, 25, 25));
    lrectv.push_back(Rectangle(25, 25, 25, 25));
    BOOST_CHECK_EQUAL(lrect, lrectv);
}

/*
BOOST_AUTO_TEST_CASE(timeline_ycomputing_0)
{
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);

    list<Rectangle*> lrect;
    lrect.push_back(new Rectangle(0, 0, 25, 100));
    lrect.push_back(new Rectangle(25, 0, 25, 100));
    lrect.push_back(new Rectangle(60, 0, 20, 100));
    lrect.push_back(new Rectangle(80, 0, 20, 100));
    lrect.push_back(new Rectangle(70, 0, 20, 100));
    list<Crop*> l_crops;
    for (Rectangle* rect: lrect) {
        Crop* ncrop_p = new Crop();
        ncrop_p->set_shape(rect);
        ncrop_p->set_plot(plot);
        l_crops.push_back(ncrop_p);
    }

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0, 0.25));
            break;
        case 1:
            refcoords.push_back(make_pair(0.25, 0.25));
            break;
        case 2:
            refcoords.push_back(make_pair(0.60, 0.20));
            break;
        case 3:
            refcoords.push_back(make_pair(0.70, 0.20));
            break;
        case 4:
            refcoords.push_back(make_pair(0.80, 0.20));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_1)
{
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);

    list<Rectangle*> lrect;
    lrect.push_back(new Rectangle(0, 0, 25, 100));
    lrect.push_back(new Rectangle(25, 0, 25, 100));
    lrect.push_back(new Rectangle(60, 0, 20, 50));
    lrect.push_back(new Rectangle(60, 50, 20, 50));
    lrect.push_back(new Rectangle(80, 0, 20, 100));
    lrect.push_back(new Rectangle(60, 0, 20, 100));
    list<Crop*> l_crops;
    for (Rectangle* rect: lrect) {
        Crop* ncrop_p = new Crop();
        ncrop_p->set_shape(rect);
        ncrop_p->set_plot(plot);
        l_crops.push_back(ncrop_p);
    }

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0, 0.25));
            break;
        case 1:
            refcoords.push_back(make_pair(0.25, 0.25));
            break;
        case 2:
            refcoords.push_back(make_pair(0.60, 0.1));
            break;
        case 3:
            refcoords.push_back(make_pair(0.70, 0.1));
            break;
        case 4:
            refcoords.push_back(make_pair(0.80, 0.2));
            break;
        case 5:
            refcoords.push_back(make_pair(0.60, 0.2));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }

}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_2)
{
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);

    list<Rectangle*> lrect;
    lrect.push_back(new Rectangle(0, 0, 25, 100));
    lrect.push_back(new Rectangle(25, 0, 25, 100));
    lrect.push_back(new Rectangle(60, 0, 20, 50));
    lrect.push_back(new Rectangle(70, 50, 10, 50));
    lrect.push_back(new Rectangle(80, 0, 20, 100));
    lrect.push_back(new Rectangle(60, 0, 20, 100));
    list<Crop*> l_crops;
    for (Rectangle* rect: lrect) {
        Crop* ncrop_p = new Crop();
        ncrop_p->set_shape(rect);
        ncrop_p->set_plot(plot);
        l_crops.push_back(ncrop_p);
    }

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0, 0.25));
            break;
        case 1:
            refcoords.push_back(make_pair(0.25, 0.25));
            break;
        case 2:
            refcoords.push_back(make_pair(0.60, 0.1));
            break;
        case 3:
            refcoords.push_back(make_pair(0.75, 0.1));
            break;
        case 4:
            refcoords.push_back(make_pair(0.80, 0.2));
            break;
        case 5:
            refcoords.push_back(make_pair(0.60, 0.2));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_3)
{
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);

    list<Rectangle*> lrect;
    lrect.push_back(new Rectangle(0, 0, 25, 100));
    lrect.push_back(new Rectangle(25, 0, 25, 100));
    lrect.push_back(new Rectangle(60, 0, 20, 50));
    lrect.push_back(new Rectangle(60, 50, 20, 50));
    lrect.push_back(new Rectangle(80, 0, 20, 100));
    lrect.push_back(new Rectangle(70, 0, 20, 100));
    list<Crop*> l_crops;
    for (Rectangle* rect: lrect) {
        Crop* ncrop_p = new Crop();
        ncrop_p->set_shape(rect);
        ncrop_p->set_plot(plot);
        l_crops.push_back(ncrop_p);
    }

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0, 0.25));
            break;
        case 1:
            refcoords.push_back(make_pair(0.25, 0.25));
            break;
        case 2:
            refcoords.push_back(make_pair(0.6, 0.1));
            break;
        case 3:
            refcoords.push_back(make_pair(0.7, 0.1));
            break;
        case 4:
            refcoords.push_back(make_pair(0.80, 0.20));
            break;
        case 5:
            refcoords.push_back(make_pair(0.65, 0.10));
            refcoords.push_back(make_pair(0.80, 0.10));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
    //self.assertEqual(timereprs, [[(0.0, 0.25)], [(0.25, 0.25)],
    //                             [(0.6, 0.05), (0.7, 0.05)],
    //                             [(0.65, 0.05), (0.75, 0.05)],
    //                             [(0.8, 0.2)], [(0.7, 0.2)]])
}


BOOST_AUTO_TEST_CASE(timeline_ycomputing_4)
{
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);

    list<Rectangle*> lrect;
    lrect.push_back(new Rectangle(0, 0, 25, 25));
    lrect.push_back(new Rectangle(75, 75, 25, 25));
    lrect.push_back(new Rectangle(50, 50, 25, 25));
    lrect.push_back(new Rectangle(25, 50, 25, 25));
    lrect.push_back(new Rectangle(50, 25, 25, 25));
    lrect.push_back(new Rectangle(75, 25, 25, 25));
    lrect.push_back(new Rectangle(50, 75, 25, 25));
    lrect.push_back(new Rectangle(25, 0, 25, 25));
    lrect.push_back(new Rectangle(0, 75, 25, 25));
    lrect.push_back(new Rectangle(0, 25, 25, 25));
    list<Crop*> l_crops;
    for (Rectangle* rect: lrect) {
        Crop* ncrop_p = new Crop();
        ncrop_p->set_shape(rect);
        ncrop_p->set_plot(plot);
        l_crops.push_back(ncrop_p);
    }

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0, 0.0625));
            break;
        case 1:
            refcoords.push_back(make_pair(0.9375, 0.0625));
            break;
        case 2:
            refcoords.push_back(make_pair(0.625, 0.0625));
            break;
        case 3:
            refcoords.push_back(make_pair(0.5625, 0.0625));
            break;
        case 4:
            refcoords.push_back(make_pair(0.375, 0.0625));
            break;
        case 5:
            refcoords.push_back(make_pair(0.4375, 0.0625));
            break;
        case 6:
            refcoords.push_back(make_pair(0.875, 0.0625));
            break;
        case 7:
            refcoords.push_back(make_pair(0.0625, 0.0625));
            break;
        case 8:
            refcoords.push_back(make_pair(0.75, 0.0625));
            break;
        case 9:
            refcoords.push_back(make_pair(0.25, 0.0625));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_5)
{
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);

    list<Rectangle*> lrect;
    lrect.push_back(new Rectangle(0, 0, 25, 25));
    lrect.push_back(new Rectangle(75, 75, 25, 25));
    lrect.push_back(new Rectangle(50, 50, 25, 25));
    lrect.push_back(new Rectangle(25, 50, 25, 25));
    lrect.push_back(new Rectangle(75, 25, 25, 25));
    lrect.push_back(new Rectangle(50, 75, 25, 25));
    lrect.push_back(new Rectangle(0, 25, 25, 25));
    list<Crop*> l_crops;
    for (Rectangle* rect: lrect) {
        Crop* ncrop_p = new Crop();
        ncrop_p->set_shape(rect);
        ncrop_p->set_plot(plot);
        l_crops.push_back(ncrop_p);
    }

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0, 0.0625));
            break;
        case 1:
            refcoords.push_back(make_pair(0.9375, 0.0625));
            break;
        case 2:
            refcoords.push_back(make_pair(0.625, 0.0625));
            break;
        case 3:
            refcoords.push_back(make_pair(0.5625, 0.0625));
            break;
        case 4:
            refcoords.push_back(make_pair(0.4375, 0.0625));
            break;
        case 5:
            refcoords.push_back(make_pair(0.875, 0.0625));
            break;
        case 6:
            refcoords.push_back(make_pair(0.25, 0.0625));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}
*/

BOOST_AUTO_TEST_CASE(timeline_ycomputing_6)
{
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);

    list<Rectangle*> lrect;
    lrect.push_back(new Rectangle(0, 0, 25, 25));
    lrect.push_back(new Rectangle(75, 75, 25, 25));
    lrect.push_back(new Rectangle(50, 50, 50, 25));
    lrect.push_back(new Rectangle(50, 50, 25, 25));
    lrect.push_back(new Rectangle(75, 50, 25, 25));
    lrect.push_back(new Rectangle(0, 0, 100, 100));
    list<Crop*> l_crops;
    for (Rectangle* rect: lrect) {
        Crop* ncrop_p = new Crop();
        ncrop_p->set_shape(rect);
        ncrop_p->set_plot(plot);
        l_crops.push_back(ncrop_p);
    }

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        const Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0.0, (float)1/16));
            break;
        case 1:
            refcoords.push_back(make_pair(1-(float)1/16, (float)1/16));
            break;
        case 2:
            refcoords.push_back(make_pair(1-(float)3/16, (float)1/8));
            break;
        case 3:
            refcoords.push_back(make_pair(1-(float)3/16, (float)1/16));
            break;
        case 4:
            refcoords.push_back(make_pair(1-(float)2/16, (float)1/16));
            break;
        case 5:
            refcoords.push_back(make_pair(0.0, 1.0));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_7)
{
    Plot plot = Plot("pl", "plot", "", 90, 90, 0, 0);

    list<Crop*> l_crops;
    for(int x=0; x<3; ++x) {
        for(int y=0; y<3; ++y) {
            Crop* ncrop_p = new Crop();
            ncrop_p->set_shape(new Rectangle(30*x, 30*y, 30, 30));
            ncrop_p->set_plot(plot);
            l_crops.push_back(ncrop_p);
        }
    }

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        const Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0.0, 1.0/9));
            break;
        case 1:
            refcoords.push_back(make_pair(1.0/3, 1.0/9));
            break;
        case 2:
            refcoords.push_back(make_pair(2.0/3, 1.0/9));
            break;
        case 3:
            refcoords.push_back(make_pair(1.0/9, 1.0/9));
            break;
        case 4:
            refcoords.push_back(make_pair(4.0/9, 1.0/9));
            break;
        case 5:
            refcoords.push_back(make_pair(7.0/9, 1.0/9));
            break;
        case 6:
            refcoords.push_back(make_pair(2.0/9, 1.0/9));
            break;
        case 7:
            refcoords.push_back(make_pair(5.0/9, 1.0/9));
            break;
        case 8:
            refcoords.push_back(make_pair(8.0/9, 1.0/9));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_8)
{
    Plot plot = Plot("pl", "plot", "", 90, 90, 0, 0);

    list<Crop*> l_crops;
    for(int x=0; x<3; ++x) {
        for(int y=0; y<3; ++y) {
            Crop* ncrop_p = new Crop();
            ncrop_p->set_shape(new Rectangle(30*x, 30*y, 30, 30));
            ncrop_p->set_plot(plot);
            l_crops.push_back(ncrop_p);
        }
    }
    Crop* ncrop_p = new Crop();
    ncrop_p->set_shape(new Rectangle(15, 15, 30, 30));
    ncrop_p->set_plot(plot);
    l_crops.push_back(ncrop_p);

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        const Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0.0, 1.0/9));
            break;
        case 1:
            refcoords.push_back(make_pair(1.0/3, 1.0/9));
            break;
        case 2:
            refcoords.push_back(make_pair(2.0/3, 1.0/9));
            break;
        case 3:
            refcoords.push_back(make_pair(1.0/9, 1.0/9));
            break;
        case 4:
            refcoords.push_back(make_pair(4.0/9, 1.0/9));
            break;
        case 5:
            refcoords.push_back(make_pair(7.0/9, 1.0/9));
            break;
        case 6:
            refcoords.push_back(make_pair(2.0/9, 1.0/9));
            break;
        case 7:
            refcoords.push_back(make_pair(5.0/9, 1.0/9));
            break;
        case 8:
            refcoords.push_back(make_pair(8.0/9, 1.0/9));
            break;
        case 9:
            refcoords.push_back(make_pair(3.0/36, 1.0/18));
            refcoords.push_back(make_pair(15.0/36, 1.0/18));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}

/* Deactivated for now
 * TODO: reactivate it
BOOST_AUTO_TEST_CASE(timeline_ycomputing_9)
{
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);

    list<Crop*> l_crops;
    list<Rectangle*> lrect;
    for (int x=0; x<2; ++x) {
        for (int y=0; y<2; ++y) {
            Crop* ncrop_p = new Crop();
            ncrop_p->set_shape(new Rectangle(50*x, 50*y, 50, 50));
            ncrop_p->set_plot(plot);
            l_crops.push_back(ncrop_p);
        }
    }
    Crop* ncrop_p = new Crop();
    ncrop_p->set_shape(new Rectangle(25, 25, 50, 50));
    ncrop_p->set_plot(plot);
    l_crops.push_back(ncrop_p);

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0.0, 1.0/4));
            break;
        case 1:
            refcoords.push_back(make_pair(1.0/4, 1.0/4));
            break;
        case 2:
            refcoords.push_back(make_pair(1.0/2, 1.0/4));
            break;
        case 3:
            refcoords.push_back(make_pair(3.0/4, 1.0/4));
            break;
        case 4:
            refcoords.push_back(make_pair(4.0/9, 1.0/8));
            refcoords.push_back(make_pair(4.0/9, 1.0/8));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}
*/

BOOST_AUTO_TEST_CASE(timeline_ycomputing_10)
{
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);

    list<Rectangle*> lrect;
    lrect.push_back(new Rectangle(0, 0, 50, 100));
    lrect.push_back(new Rectangle(50, 0, 50, 100));
    lrect.push_back(new Rectangle(0, 50, 100, 50));
    lrect.push_back(new Rectangle(0, 0, 100, 50));
    list<Crop*> l_crops;
    for (Rectangle* rect: lrect) {
        Crop* ncrop_p = new Crop();
        ncrop_p->set_shape(rect);
        ncrop_p->set_plot(plot);
        l_crops.push_back(ncrop_p);
    }

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        const Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0.0, 0.25));
            refcoords.push_back(make_pair(0.75, 0.25));
            break;
        case 1:
            refcoords.push_back(make_pair(0.25, 0.5));
            break;
        case 2:
            refcoords.push_back(make_pair(0.5, 0.5));
            break;
        case 3:
            refcoords.push_back(make_pair(0.0, 0.5));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }

    //self.assertEqual(timereprs, [[(0.0, 0.25), (0.5, 0.25)], [(0.25, 0.25), (0.75, 0.25)],
    //                             [(0.5, 0.5)], [(0.0, 0.5)]])
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_11)
{
    //No crop. Must not crash...
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);


    list<Crop*> l_crops;
    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    BOOST_CHECK(partitions.empty());
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_12)
{
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);

    list<Rectangle*> lrect;
    lrect.push_back(new Rectangle(0, 0, 100, 100));
    list<Crop*> l_crops;
    for (Rectangle* rect: lrect) {
        Crop* ncrop_p = new Crop();
        ncrop_p->set_shape(rect);
        ncrop_p->set_plot(plot);
        l_crops.push_back(ncrop_p);
    }

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        const Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0.0, 1));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_13)
{
    Plot plot = Plot("pl", "plot", "", 100, 100, 0, 0);

    list<Rectangle*> lrect;
    lrect.push_back(new Rectangle(0, 0, 100, 100));
    lrect.push_back(new Rectangle(0, 50, 100, 50));
    list<Crop*> l_crops;
    for (Rectangle* rect: lrect) {
        Crop* ncrop_p = new Crop();
        ncrop_p->set_shape(rect);
        ncrop_p->set_plot(plot);
        l_crops.push_back(ncrop_p);
    }

    list<Rectangle> partitions = compute_partitions(l_crops, plot);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int icrop = 0; icrop < l_crops.size(); ++icrop) {
        const Crop* crop_p = l_crops.front();
        l_crops.pop_front();
        list< pair<float, float> > ycoords = compute_timerepr(*crop_p, partitions);
        list< pair<float, float> > refcoords;
        switch (icrop) {
        case 0:
            refcoords.push_back(make_pair(0.0, 1));
            break;
        case 1:
            refcoords.push_back(make_pair(0.5, 1));
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}
