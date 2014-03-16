
#ifndef PLANT_H
#define PLANT_H

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
        Plant();
        Plant(string key, string name, string note="", string color="");
        void add_var(string key, string name, string note="");
        string get_color_str() const;
        void set_color_str(string color);
        Vars& get_vars();
};
extern Plant NullPlant;


#endif //PLANT_H
