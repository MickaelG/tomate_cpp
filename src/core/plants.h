
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
    Plant& add(std::string key, const std::string& name, const std::string& note="");
    Plant* find(const std::string& key);
    bool delete_plant(int iplant);
    bool is_used(const Plant& plant) const;
    my_iterator<Plant> begin();
    my_iterator<Plant> end();
    my_const_iterator<Plant> begin() const;
    my_const_iterator<Plant> end() const;
    const Plant& operator[](int index) const;
    Plant& operator[](int index);
    int size() const;

private:
    void remove(int iplant);

    const Crops& crops;
    std::vector<std::unique_ptr<Plant> > _vplants;
};

#endif //PLANTS_H
