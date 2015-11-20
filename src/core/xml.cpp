#include "pugixml.hpp"
#include "xml.h"
#include "dataset.h"
#include "plot.h"
#include "plant.h"
#include "crops.h"
using namespace pugi;

#include <unordered_map>

bg::date get_date(xml_node &elem_xml, string attribute_name)
{
    string date_str = elem_xml.attribute(attribute_name.c_str()).value();
    bg::date date(bg::not_a_date_time);
    if (date_str != "")
    {
        date = bg::from_string(date_str);
    }
    return date;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to);

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

int xml_read_data(string filename, Dataset& dataset)
{
    xml_document doc;
    xml_parse_result load_res = doc.load_file(filename.c_str());
    if (!load_res)
    {
        if (load_res.status == status_file_not_found)
        {
            std::cout << "File does not exists." << std::endl;
            return -1;
        }
        else
        {
            std::cout << "Error while parsing xml: " << load_res.description() << " (offset: " << load_res.offset << ") " << std::endl;
            return -2;
        }
    }
    dataset.set_filename(filename);

    unordered_map<string, vector<float>> subd_data;

    xml_node root_node = doc.child("sfg");
    //for (elem_xml = root_node.child("plots").first_child(); elem_xml; elem_xml = elem_xml.next_sibling())
    for (xml_node elem_xml: root_node.child("plots").children())
    {
        string key = elem_xml.attribute("key").value();
        string name = elem_xml.attribute("name").value();
        string descr = elem_xml.attribute("descr").value();
        float width = elem_xml.attribute("width").as_float(-1);
        float height = elem_xml.attribute("height").as_float(-1);
        float posx = elem_xml.attribute("posx").as_float(0);
        float posy = elem_xml.attribute("posy").as_float(0);
        dataset.get_plots().add(key, name, descr, width, height, posx, posy);

        //Saved for conversion from 0.1 to 0.2
        int isubd = 0;
        for (xml_node subd_xml: elem_xml.children())
        {
            isubd ++;
            float width = subd_xml.attribute("width").as_float(-1);
            float height = subd_xml.attribute("height").as_float(-1);
            float posx = subd_xml.attribute("posx").as_float(0);
            float posy = subd_xml.attribute("posy").as_float(0);
            std::stringstream sstm;
            sstm << key << "-" << isubd;
            string all_key = sstm.str();
            subd_data[all_key] = {posx, posy, width, height};
        }
    }

    for (xml_node elem_xml : root_node.child("plants").children())
    {
        string key = elem_xml.attribute("key").value();
        string name = elem_xml.attribute("name").value();
        string note = elem_xml.attribute("note").value();
        replaceAll(note, "\\n", "\n");
        string color = elem_xml.attribute("color").value();
        Plant& plant = dataset.get_plants().add(key, name, note);
        plant.set_color_str(color);
        for (xml_node var_xml: elem_xml.children())
        {
            string varkey = var_xml.attribute("key").value();
            string varname = var_xml.attribute("name").value();
            string note = var_xml.attribute("note").value();
            plant.add_var(varkey, varname, note);
        }
    }

    for (xml_node elem_xml: root_node.child("crops").children())
    {
        bg::date start_date = get_date(elem_xml, "start_date");
        bg::date end_date = get_date(elem_xml, "end_date");
        bg::date planned_start_date = get_date(elem_xml, "planned_start_date");
        bg::date planned_end_date = get_date(elem_xml, "planned_end_date");

        string plotkey = elem_xml.attribute("plot").value();
        string plantkey = elem_xml.attribute("plant").value();
        string varkey = elem_xml.attribute("var").value();
        string note = elem_xml.attribute("note").value();

        Plant* p_plant = dataset.get_plants().find(plantkey);
        if (p_plant == nullptr)
        {
            std::cout << "Error: plant " << plantkey << " does not exist (xml offset " << load_res.offset << ")" << std::endl;
            continue;
        }

        float width = elem_xml.attribute("width").as_float(-1);
        float height = elem_xml.attribute("height").as_float(-1);
        float posx = elem_xml.attribute("posx").as_float(0);
        float posy = elem_xml.attribute("posy").as_float(0);

        Plot* p_plot = dataset.get_plots().find(plotkey);
        if (!p_plot)
        {
            size_t delim_pos = plotkey.find("-");
            if (width < 0 && delim_pos != string::npos)
            {
                string mainplotkey = plotkey.substr(0, delim_pos);
                if (subd_data.count(plotkey))
                {
                    p_plot = dataset.get_plots().find(mainplotkey);
                    if (!p_plot)
                    {
                        std::cout << "Error: conversion from 0.1, plot " << mainplotkey << " does not exist (xml offset " << load_res.offset << ")" << std::endl;
                    }
                    posx = subd_data[plotkey][0];
                    posy = subd_data[plotkey][1];
                    width = subd_data[plotkey][2];
                    height = subd_data[plotkey][3];
                }
                else
                {
                    std::cout << "Error: conversion from 0.1, subplot data " << plotkey << " does not exist (xml offset " << load_res.offset << ")" << std::endl;
                    continue;
                }

            }
            else
            {
                std::cout << "Error: plot " << plotkey << " does not exist (xml offset " << load_res.offset << ")" << std::endl;
                continue;
            }
        }
        Rectangle rect(posx, posy, width, height);

        Crop& crop = dataset.get_crops().add(start_date, end_date,
                                             planned_start_date, planned_end_date,
                                             dataset.get_plants().find(plantkey), varkey, p_plot, note, rect);
        for (xml_node action_xml: elem_xml.children())
        {
            bg::date date = get_date(action_xml, "date");
            string note = action_xml.attribute("note").value();
            crop.add_action(date, note);
        }

    }
    return 0;
}

void add_date_attribute(xml_node &elem_node, string attribute_name, bg::date date)
{
    if (date != bg::date(bg::not_a_date_time))
    {
        elem_node.append_attribute(attribute_name.c_str()) = bg::to_iso_extended_string(date).c_str();
    }
}

void add_str_attribute(xml_node &elem_node, string attribute_name, string attribute_value)
{
    if (attribute_value != "")
    {
        elem_node.append_attribute(attribute_name.c_str()) = attribute_value.c_str();
    }
}

void add_float_attribute(xml_node &elem_node, string attribute_name, float attribute_value)
{
    elem_node.append_attribute(attribute_name.c_str()) = attribute_value;
}

int xml_write_data(string filename, const Dataset& dataset)
{
    xml_document doc;

    xml_node root_node = doc.append_child("sfg");
    root_node.append_attribute("v") = "0.2";

    xml_node plot_node = root_node.append_child("plots");
    for (const Plot& plot: dataset.get_plots())
    {
        xml_node elem_node = plot_node.append_child("plot");
        elem_node.append_attribute("key") = plot.get_key().c_str();
        elem_node.append_attribute("name") = plot.get_name().c_str();
        elem_node.append_attribute("descr") = plot.get_note().c_str();

        float width = plot.get_shape()->get_width();
        float height = plot.get_shape()->get_height();
        if (width > 0 && height > 0)
        {
            add_float_attribute(elem_node, "posx", plot.get_shape()->get_min_x());
            add_float_attribute(elem_node, "posy", plot.get_shape()->get_min_y());
            add_float_attribute(elem_node, "width", width);
            add_float_attribute(elem_node, "height", height);
        }
    }
    xml_node plant_node = root_node.append_child("plants");
    for (const Plant& plant: dataset.get_plants())
    {
        xml_node elem_node = plant_node.append_child("plant");
        elem_node.append_attribute("key") = plant.get_key().c_str();
        elem_node.append_attribute("name") = plant.get_name().c_str();
        add_str_attribute(elem_node, "color", plant.get_color_str());
        string note = plant.get_note();
        replaceAll(note, "\n", "\\n");
        add_str_attribute(elem_node, "note", note);
        for (const Var& var: plant.get_vars())
        {
            xml_node var_node = elem_node.append_child("var");
            var_node.append_attribute("key") = var.get_key().c_str();
            var_node.append_attribute("name") = var.get_name().c_str();
            add_str_attribute(var_node, "note", var.get_note());
        }
    }
    xml_node crop_node = root_node.append_child("crops");
    for (const Crop& crop: dataset.get_crops())
    {
        xml_node elem_node = crop_node.append_child("crop");
        for (Crop::DateSel which_date: {Crop::DateSel::START, Crop::DateSel::END,
                                        Crop::DateSel::PLANNED_START, Crop::DateSel::PLANNED_END} )
        {
            string which_str;
            switch (which_date) {
            case Crop::DateSel::START:
                which_str = "start";
                break;
            case Crop::DateSel::END:
                which_str = "end";
                break;
            case Crop::DateSel::PLANNED_START:
                which_str = "planned_start";
                break;
            case Crop::DateSel::PLANNED_END:
                which_str = "planned_end";
                break;
            }

            add_date_attribute(elem_node, which_str + "_date", crop.get_date(which_date));
        }
        elem_node.append_attribute("plant") = crop.get_plant().get_key().c_str();
        add_str_attribute(elem_node, "var", crop.get_varkey());

        const Plot& plot = crop.get_plot();
        float plot_width = 0;
        float plot_height = 0;
        elem_node.append_attribute("plot") = plot.get_key().c_str();
        plot_width = plot.get_shape()->get_width();
        plot_height = plot.get_shape()->get_height();

        float width = crop.get_shape()->get_width();
        float height = crop.get_shape()->get_height();
        float posx = crop.get_shape()->get_min_x();
        float posy = crop.get_shape()->get_min_y();
        if (width > 0 && height > 0 &&
            (posx != 0 || posy != 0 || width != plot_width || height != plot_height))

        {
            add_float_attribute(elem_node, "posx", posx);
            add_float_attribute(elem_node, "posy", posy);
            add_float_attribute(elem_node, "width", width);
            add_float_attribute(elem_node, "height", height);
        }

        add_str_attribute(elem_node, "note", crop.get_note());

        for (CropAction act: crop.get_actions())
        {
            xml_node act_node = elem_node.append_child("action");
            add_date_attribute(act_node, "date", act.get_date());
            add_str_attribute(act_node, "note", act.get_note());
        }

    }
    bool wsuccess = doc.save_file(filename.c_str(), "  ");
    if (!wsuccess) {
        return -1;
    }

    return 0;
}

