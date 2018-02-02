#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hello
#include <boost/test/unit_test.hpp>

#include "dataset.h"
#include "plot.h"
#include "plant.h"
#include "calendar.h"
#include "xml.h"
#include "partitionset.h"

BOOST_AUTO_TEST_CASE(crops)
{
    PlantSpecies plant("tomate");
    Plot plot("Carré1", "description du carré 1", std::make_unique< Rectangle >(8,9,2,3));
    bg::date start_date(2013, 3, 3);
    bg::date end_date(2013, 3, 23);
    Crop crop(start_date, end_date, &plant, CropLocation());
    BOOST_CHECK(crop.get_plant().get_name() == "tomate");
    plant.set_name("Concombre");
    BOOST_CHECK(crop.get_plant().get_name() == "Concombre");
    PlantSpecies plant2("radis");
    crop.set_plant(plant2);
    BOOST_CHECK(crop.get_plant().get_name() == "radis");
}

#include <fstream>
#include <sstream>
string slurp(ifstream& in) {
    stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
}

BOOST_AUTO_TEST_CASE(xml_conv_from_02)
{
    Dataset dataset;
    xml_read_data("../testfiles/data_v0.2.sfg", dataset);
    BOOST_CHECK(dataset.get_plots()[1].get_name() == "Plot1");
    xml_write_data("data_out.sfg", dataset);
    ifstream ifs1("../testfiles/data_v0.2.out_v0.4.sfg");
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
    BOOST_CHECK(dataset.get_plots()[1].get_name() == "Plot1");
    xml_write_data("data_out.sfg", dataset);
    ifstream ifs1("../testfiles/data_v0.1.out_v0.4.sfg");
    ifstream ifs2("data_out.sfg");
    string str1;
    string str2;
    while(getline(ifs1, str1) && getline(ifs2, str2)) {
        BOOST_REQUIRE_EQUAL(str1, str2);
    }
    BOOST_CHECK(!(ifs1 && ifs2));
}

BOOST_AUTO_TEST_CASE(xml_conv_from_03)
{
    Dataset dataset;
    xml_read_data("../testfiles/data_v0.3.sfg", dataset);
    BOOST_CHECK(dataset.get_plots()[1].get_name() == "Plot1");
    xml_write_data("data_out.sfg", dataset);
    ifstream ifs1("../testfiles/data_v0.3.out_v0.4.sfg");
    ifstream ifs2("data_out.sfg");
    string str1;
    string str2;
    while(getline(ifs1, str1) && getline(ifs2, str2)) {
        BOOST_REQUIRE_EQUAL(str1, str2);
    }
    BOOST_CHECK(!(ifs1 && ifs2));
}

BOOST_AUTO_TEST_CASE(xml_v04)
{
    Dataset dataset;
    xml_read_data("../testfiles/data_v0.4.sfg", dataset);
    BOOST_CHECK(dataset.get_plots()[1].get_name() == "Plot1");
    xml_write_data("data_out.sfg", dataset);
    ifstream ifs1("../testfiles/data_v0.4.sfg");
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
    PlantSpecies plant("plant_name");
   
    //Crop with only a start date
    Crop crop1(bg::date(2012, 10, 02), bg::date(), &plant, CropLocation());
    BOOST_CHECK(crop1.is_active_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(crop1.is_active_at_date(bg::date(bg::day_clock::local_day())));
    BOOST_CHECK(!crop1.is_active_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(!crop1.is_planned_at_date(bg::date(2012, 10, 03)));
    BOOST_CHECK(!crop1.is_planned_at_date(bg::date(2012, 10, 01)));
    BOOST_CHECK(!crop1.is_in_year_started_by(bg::date(2011, 01, 01)));
    BOOST_CHECK(crop1.is_in_year_started_by(bg::date(2012, 01, 01)));
    //BOOST_CHECK(!crop1.is_in_year_started_by(bg::date(2013, 01, 01)));

    //Crop with start date and end date
    Crop crop2(bg::date(2012, 10, 02), bg::date(2012, 11, 22), &plant, CropLocation());
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
    Crop crop3(bg::date(), bg::date(), bg::date(2012, 10, 02), bg::date(2012, 11, 22), &plant, CropLocation());
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
    Crop crop4(bg::date(2012, 10, 15), bg::date(2012, 11, 28),
               bg::date(2012, 10, 02), bg::date(2012, 11, 22),
               &plant, CropLocation());
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
    Crop crop5(bg::date(2012, 10, 15), bg::date(), bg::date(), bg::date(2012, 11, 22), &plant, CropLocation());
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
    Crop crop6(bg::date(2012, 10, 15), bg::date(2012, 11, 01),
               bg::date(2012, 10, 1), bg::date(),
               &plant, CropLocation(Rectangle()));
    BOOST_CHECK(!crop6.is_active_at_date(bg::date(2012, 10, 14)));
    BOOST_CHECK(crop6.is_active_at_date(bg::date(2012, 10, 16)));
    BOOST_CHECK(crop6.is_active_at_date(bg::date(2012, 10, 30)));
    BOOST_CHECK(!crop6.is_planned_at_date(bg::date(2012, 9, 30)));
    BOOST_CHECK(crop6.is_planned_at_date(bg::date(2012, 10, 2)));
    BOOST_CHECK(!crop6.is_in_year_started_by(bg::date(2011, 01, 01)));
    BOOST_CHECK(crop6.is_in_year_started_by(bg::date(2012, 01, 01)));
    BOOST_CHECK(!crop6.is_in_year_started_by(bg::date(2013, 01, 01)));

}

BOOST_AUTO_TEST_CASE(plots)
{
    Dataset data;
    Plot& plot = data.get_plots().add("test");
    Plot& plt2 = data.get_plots().add("test2");

    BOOST_CHECK_EQUAL(data.get_plots().size(), 2);
    auto plot_it = data.get_plots().begin();
    BOOST_CHECK_EQUAL(plot_it->get_name(), "test");
    BOOST_CHECK_EQUAL((++plot_it)->get_name(), "test2");

    data.get_plots().remove(0);

    BOOST_CHECK_EQUAL(data.get_plots().size(), 1);
    BOOST_CHECK_EQUAL(data.get_plots().begin()->get_name(), "test2");

    data.get_plots().remove(0);

    BOOST_CHECK_EQUAL(data.get_plots().size(), 0);
}

BOOST_AUTO_TEST_CASE(plants)
{
    Dataset data;
    Plant& plant = data.get_plants().add("plant1");
    Plot plot("Carré1", "description du carré 1", std::make_unique< Rectangle >(8,9,2,3));
    bg::date start_date(2013, 3, 3);
    bg::date end_date(2013, 3, 23);

    BOOST_CHECK_EQUAL(data.get_plants().is_used(plant), false);
    data.get_crops().add(start_date, end_date, bg::date(), bg::date(), &plant, CropLocation());
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

static void check_converter(const Rectangle& master,
                            const vector<Rectangle>& input,
                            const vector<pair<float, float> >& ref)
{
    BOOST_CHECK_EQUAL(input.size(), ref.size());
    _2dTo1dConverter converter(master, input);
    for (int irect = 0; irect < input.size(); ++irect) {
        pair<float, float> ycoords = converter.get_position(input[irect]);
        BOOST_CHECK_EQUAL(ycoords, ref[irect]);
    }

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
            refcoords = make_pair(0, 0.25);
            break;
        case 1:
            refcoords = make_pair(0.25, 0.25);
            break;
        case 2:
            refcoords = make_pair(0.60, 0.20);
            break;
        case 3:
            refcoords = make_pair(0.70, 0.20);
            break;
        case 4:
            refcoords = make_pair(0.80, 0.20);
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
            refcoords = make_pair(0, 0.25);
            break;
        case 1:
            refcoords = make_pair(0.25, 0.25);
            break;
        case 2:
            refcoords = make_pair(0.60, 0.1);
            break;
        case 3:
            refcoords = make_pair(0.70, 0.1);
            break;
        case 4:
            refcoords = make_pair(0.80, 0.2);
            break;
        case 5:
            refcoords = make_pair(0.60, 0.2);
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
            refcoords = make_pair(0, 0.25);
            break;
        case 1:
            refcoords = make_pair(0.25, 0.25);
            break;
        case 2:
            refcoords = make_pair(0.60, 0.1);
            break;
        case 3:
            refcoords = make_pair(0.75, 0.1);
            break;
        case 4:
            refcoords = make_pair(0.80, 0.2);
            break;
        case 5:
            refcoords = make_pair(0.60, 0.2);
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
            refcoords = make_pair(0, 0.25);
            break;
        case 1:
            refcoords = make_pair(0.25, 0.25);
            break;
        case 2:
            refcoords = make_pair(0.6, 0.1);
            break;
        case 3:
            refcoords = make_pair(0.7, 0.1);
            break;
        case 4:
            refcoords = make_pair(0.80, 0.20);
            break;
        case 5:
            refcoords = make_pair(0.65, 0.10);
            refcoords = make_pair(0.80, 0.10);
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
            refcoords = make_pair(0, 0.0625);
            break;
        case 1:
            refcoords = make_pair(0.9375, 0.0625);
            break;
        case 2:
            refcoords = make_pair(0.625, 0.0625);
            break;
        case 3:
            refcoords = make_pair(0.5625, 0.0625);
            break;
        case 4:
            refcoords = make_pair(0.375, 0.0625);
            break;
        case 5:
            refcoords = make_pair(0.4375, 0.0625);
            break;
        case 6:
            refcoords = make_pair(0.875, 0.0625);
            break;
        case 7:
            refcoords = make_pair(0.0625, 0.0625);
            break;
        case 8:
            refcoords = make_pair(0.75, 0.0625);
            break;
        case 9:
            refcoords = make_pair(0.25, 0.0625);
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
            refcoords = make_pair(0, 0.0625);
            break;
        case 1:
            refcoords = make_pair(0.9375, 0.0625);
            break;
        case 2:
            refcoords = make_pair(0.625, 0.0625);
            break;
        case 3:
            refcoords = make_pair(0.5625, 0.0625);
            break;
        case 4:
            refcoords = make_pair(0.4375, 0.0625);
            break;
        case 5:
            refcoords = make_pair(0.875, 0.0625);
            break;
        case 6:
            refcoords = make_pair(0.25, 0.0625);
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}
*/

BOOST_AUTO_TEST_CASE(timeline_ycomputing_6)
{
    Plot plot = Plot("plot", "", 100, 100, 0, 0);

    vector<Rectangle> lrect;
    lrect.push_back(Rectangle(0, 0, 25, 25));
    lrect.push_back(Rectangle(75, 75, 25, 25));
    lrect.push_back(Rectangle(50, 50, 50, 25));
    lrect.push_back(Rectangle(50, 50, 25, 25));
    lrect.push_back(Rectangle(75, 50, 25, 25));
    lrect.push_back(Rectangle(0, 0, 100, 100));

    _2dTo1dConverter converter(plot.get_shape(), lrect);
    //cout << "DEBUG partitions:" << partitions << endl;
    for (int irect = 0; irect < lrect.size(); ++irect) {
        const Rectangle& rect = lrect[irect];
        pair<float, float> ycoords = converter.get_position(rect);
        pair<float, float> refcoords;
        switch (irect) {
        case 0:
            refcoords = make_pair(0.0, (float)1/16);
            break;
        case 1:
            refcoords = make_pair(1-(float)1/16, (float)1/16);
            break;
        case 2:
            refcoords = make_pair(1-(float)3/16, (float)1/8);
            break;
        case 3:
            refcoords = make_pair(1-(float)3/16, (float)1/16);
            break;
        case 4:
            refcoords = make_pair(1-(float)2/16, (float)1/16);
            break;
        case 5:
            refcoords = make_pair(0.0, 1.0);
            break;
        }
        BOOST_CHECK_EQUAL(ycoords, refcoords);
    }
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_7)
{
    Rectangle plot(90, 90);

    vector<Rectangle> rects;
    for(int x=0; x<3; ++x) {
        for(int y=0; y<3; ++y) {
            rects.push_back(Rectangle(30*x, 30*y, 30, 30));
        }
    }
    vector<pair<float, float> > refcoords = {
        make_pair(0.0, 1.0/9),
        make_pair(1.0/3, 1.0/9),
        make_pair(2.0/3, 1.0/9),
        make_pair(1.0/9, 1.0/9),
        make_pair(4.0/9, 1.0/9),
        make_pair(7.0/9, 1.0/9),
        make_pair(2.0/9, 1.0/9),
        make_pair(5.0/9, 1.0/9),
        make_pair(8.0/9, 1.0/9)
    };

    check_converter(plot, rects, refcoords);

}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_8)
{
    Rectangle plot(90, 90);

    vector<Rectangle> rects;
    list<Crop*> l_crops;
    for(int x=0; x<3; ++x) {
        for(int y=0; y<3; ++y) {
            rects.push_back(Rectangle(30*x, 30*y, 30, 30));
        }
    }
    rects.push_back(Rectangle(15, 15, 30, 30));
    vector<pair<float, float> > refcoords = {
        make_pair(-1, 1.0/9),
        make_pair(-1, 1.0/9),
        make_pair(-1, 1.0/9),
        make_pair(-1, 1.0/9),
        make_pair(-1, 1.0/9),
        make_pair(-1, 1.0/9),
        make_pair(-1, 1.0/9),
        make_pair(-1, 1.0/9),
        make_pair(-1, 1.0/9),
        make_pair(-1, 1.0/9)
    };
    check_converter(plot, rects, refcoords);
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_9)
{
    Rectangle plot(90, 90);

    vector<Rectangle> rects;
    list<Crop*> l_crops;
    for(int x=0; x<3; ++x) {
        for(int y=0; y<3; ++y) {
            rects.push_back(Rectangle(30*x, 30*y, 30, 30));
        }
    }
    rects.push_back(Rectangle(0, 15, 30, 30));
    vector<pair<float, float> > refcoords = {
        make_pair(0.0, 1.0/9),
        make_pair(1.0/9, 1.0/9),
        make_pair(2.0/3, 1.0/9),
        make_pair(2.0/9, 1.0/9),
        make_pair(4.0/9, 1.0/9),
        make_pair(7.0/9, 1.0/9),
        make_pair(1.0/3, 1.0/9),
        make_pair(5.0/9, 1.0/9),
        make_pair(8.0/9, 1.0/9),
        make_pair(1.0/18, 1.0/9)
    };
    check_converter(plot, rects, refcoords);
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_10)
{
    Rectangle plot(100, 100);

    vector<Rectangle> lrect;
    lrect.push_back(Rectangle(0, 0, 50, 100));
    lrect.push_back(Rectangle(50, 0, 50, 100));
    lrect.push_back(Rectangle(0, 50, 100, 50));
    lrect.push_back(Rectangle(0, 0, 100, 50));

    vector<pair<float, float> > refcoords = {
        make_pair(-1, 0.5),
        make_pair(-1, 0.5),
        make_pair(-1, 0.5),
        make_pair(-1, 0.5)
    };
    check_converter(plot, lrect, refcoords);
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_11)
{
    //No crop. Must not crash...
    Rectangle plot(100, 100);
    vector<Rectangle> lrect;
    vector<pair<float, float> > refcoords;
    check_converter(plot, lrect, refcoords);
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_12)
{
    Rectangle plot(100, 100);

    vector<Rectangle> lrect;
    lrect.push_back(Rectangle(0, 0, 100, 100));
    vector<pair<float, float> > refcoords = { make_pair(0, 1) };
    check_converter(plot, lrect, refcoords);
}

BOOST_AUTO_TEST_CASE(timeline_ycomputing_13)
{
    Rectangle plot(100, 100);

    vector<Rectangle> lrect;
    lrect.push_back(Rectangle(0, 0, 100, 100));
    lrect.push_back(Rectangle(0, 50, 100, 50));
    vector<pair<float, float> > refcoords = {
        make_pair(0.0, 1),
        make_pair(0.5, 0.5)
    };
    check_converter(plot, lrect, refcoords);
}

BOOST_AUTO_TEST_CASE(calendar_test_1)
{
    PlantSpecies plant("plant_name");
    Plot plot("plot_name");
    unique_ptr< Crop > crop1(new Crop(bg::date(2012, 10, 2), bg::date(), &plant, CropLocation()));
    Crops crops;
    crops.add(std::move(crop1));

    CropsCalendar TestCal(crops);
    for (auto& event: TestCal.GetEvents()) {
        BOOST_CHECK_EQUAL(event->GetStartDate(), bg::date(2012, 10, 2));
    }
    BOOST_CHECK_EQUAL(TestCal.GetEvents(bg::date(2012, 10, 2)).size(), 1);
    BOOST_CHECK_EQUAL(TestCal.GetEvents(bg::date(2012, 10, 3)).size(), 0);
    BOOST_CHECK_EQUAL(TestCal.GetEvents(bg::date(2012, 9, 1), bg::date(2012, 10, 2)).size(), 1);
    BOOST_CHECK_EQUAL(TestCal.GetEvents(bg::date(2012, 9, 1), bg::date(2012, 10, 1)).size(), 0);
}
