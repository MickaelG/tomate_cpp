
#ifndef PLANTS_H
#define PLANTS_H

#include "plant.h"

#include <vector>
#include <memory>

#include "my_iterator.h"
#include "plant.h"
class Crops;

class Plants
{
public:
    Plants(const Crops& crops);
    PlantSpecies& add(const std::string& name, const std::string& note="");
    bool delete_plant(int iplant);
    bool is_used(const Plant& plant) const;
    my_iterator<PlantSpecies> begin();
    my_iterator<PlantSpecies> end();
    my_const_iterator<PlantSpecies> begin() const;
    my_const_iterator<PlantSpecies> end() const;
    const PlantSpecies& operator[](int index) const;
    PlantSpecies& operator[](int index);
    int size() const;

private:
    void remove(int iplant);

    const Crops& crops;
    std::vector<std::unique_ptr<PlantSpecies> > _vplants;
};

#endif //PLANTS_H
