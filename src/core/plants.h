
#ifndef PLANTS_H
#define PLANTS_H

#include "keynames.h"

class Var: public KeyName
{
    private:
    public:
        Var(string key, string name, string note = "");
};

class Vars: public list<Var>
{
};


class Plant: public KeyName
{
    private:
        string color;
        Vars varlist;
    public:
        Plant() {};
        Plant(string key, string name, string note="", string color="");
        void add_var(string key, string name, string note="");
        string get_color_str() const;
        void set_color_str(string color);
        Vars& get_vars();
};
extern Plant NullPlant;

//this is dangerous to store reference from vector elements. use list.
class Plants: public list<Plant>
{
public:
    Plant& index(int data_index);
    Plant& add_plant(string key, string name);
    Plant& get_plant(string key);
};

#endif //PLANTS_H
