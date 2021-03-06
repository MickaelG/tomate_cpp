
#ifndef CROPS_H
#define CROPS_H

#include <boost/date_time.hpp>
namespace bg = boost::gregorian;

#include <string>
#include <vector>

class Plot;
class Plant;
class Shape;
#include "geometry.h"
#include "my_iterator.h"

class CropAction
{
public:
    CropAction(bg::date date, std::string note) : date(date), note(note) {}
    bg::date get_date() const { return date; }
    std::string get_note() const { return note; }
    
private:
    bg::date date;
    std::string note;
};

class CropLocation
{
public:
    CropLocation(Rectangle rect = Rectangle());
    
    Shape& get_shape();
    const Shape& get_shape() const;
    void set_shape(std::unique_ptr< Shape > in_shape);
    
private:
    std::unique_ptr< Shape > shape;
};

class Crop
{
public:
    enum class DateSel {
        INVALID,
        START,
        END,
        PLANNED_START,
        PLANNED_END
    };

    Crop(bg::date start_date, bg::date end_date,
         bg::date planned_start_date, bg::date planned_end_date,
         Plant *p_plant,
         CropLocation location,
         const std::string& note = "");
    Crop(bg::date start_date, bg::date end_date,
         Plant* p_plant,
         CropLocation location, const std::string& note = "");
    std::string get_repr() const;

    Plant& get_plant();
    const Plant& get_plant() const;
    void set_plant(Plant& plant);

    bg::date get_date(DateSel which) const;
    void set_date(DateSel which, bg::date date);
    bg::date get_virtual_end_date() const;
    bg::date get_virtual_planned_start_date() const;
    bg::date get_virtual_planned_end_date() const;
    void add_action(bg::date date, std::string note);
    std::vector<CropAction>& get_actions();
    const std::vector<CropAction>& get_actions() const;
    std::string get_note() const;
    void set_note(std::string note);
    explicit operator bool() const;
    bool is_active_at_date(bg::date date) const;
    bool is_planned_at_date(bg::date date) const;
    bool is_in_year_started_by(bg::date date) const;
    std::string description() const;
    Shape& get_shape();
    const Shape& get_shape() const;
    void set_shape(std::unique_ptr< Shape > in_shape);

private:
    void set_default_shape();

    bg::date start_date;
    bg::date end_date;
    bg::date planned_start_date;
    bg::date planned_end_date;
    Plant* p_plant;
    std::vector<CropAction> actions;
    CropLocation location;
    std::string note;
};

bool operator==(const Crop&, const Crop&);


class Crops
{
public:
    std::vector<Crop*> find_crops(const Plot& plot, bg::date date);
    std::vector<Crop*> crops_for_year(const Plot& plot, bg::date date);
    Crop& add(bg::date start_date, bg::date end_date,
              bg::date planned_start_date, bg::date planned_end_date,
              Plant *p_plant,
              CropLocation location,
              const std::string& note = "");
    Crop& add(std::unique_ptr< Crop > crop);
    bool is_used_plot(const Plot& plot) const;
    bool is_used_plant(const Plant& plant) const;
    void remove(const Crop& crop);
    my_iterator<Crop> begin();
    my_iterator<Crop> end();
    my_const_iterator<Crop> begin() const;
    my_const_iterator<Crop> end() const;
    int size() const;

private:
    std::vector< std::unique_ptr<Crop> > _vcrops;
};

#endif //CROPS_H
