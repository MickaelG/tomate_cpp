
#ifndef PLANTS_H
#define PLANTS_H

#include "keynames.h"
#include "plant.h"
#include "crops.h"

//this is dangerous to store reference from vector elements. use list.
class Plants: public list<Plant>
{
public:
    Plants(const Crops& crops);
    Plant& index(int data_index);
    const Plant& index(int data_index) const;
    Plant& add_plant(string key, string name);
    Plant& get_plant(string key);
    int delete_plant(string key);
    int delete_plant(int index);
    int delete_plant(Plant& plant);
    bool is_used(int index) const;
    bool is_used(const Plant& plant) const;

private:
    const Crops& crops;
};

#endif //PLANTS_H
