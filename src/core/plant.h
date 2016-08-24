
#ifndef PLANT_H
#define PLANT_H

#include "my_iterator.h"

#include <vector>
#include <string>

class PlantVariety;
class PlantSpecies;

class Plant
{
public:
    virtual std::string get_color_str() const = 0;
    virtual std::string get_name() const = 0;
    virtual const PlantSpecies& get_species() const = 0;
};

class PlantSpecies: public Plant
{
public:
    PlantSpecies(const PlantSpecies& other) = delete;
    PlantSpecies(std::string name, std::string note="", std::string color="");
    PlantVariety& add_var(std::string name, std::string note="");
    std::string get_color_str() const override;
    void set_color_str(std::string color);

    //Temp, for xml writer only
    const std::vector<std::unique_ptr<PlantVariety> >& get_vars() const;

    void set_name(std::string new_name);
    std::string get_name() const override;
    void set_note(std::string note);
    const std::string& get_note() const;

    const PlantSpecies& get_species() const override;

private:
    std::string name;
    std::string note;
    std::string color;
    std::vector<std::unique_ptr<PlantVariety> > vars;
};

class PlantVariety: public Plant
{
public:
    PlantVariety(PlantSpecies& species, std::string name, std::string note = "");
    void set_name(std::string new_name);
    std::string get_name() const override;
    void set_note(std::string note);
    const std::string& get_note() const;
    std::string get_color_str() const override;
    const PlantSpecies& get_species() const override;

private:
    std::string name;
    std::string note;

    PlantSpecies& species;
};

bool operator==(const Plant& lhs, const Plant& rhs);

#endif //PLANT_H
