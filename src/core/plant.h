
#ifndef PLANT_H
#define PLANT_H

#include "keynames.h"
#include "my_iterator.h"

#include <vector>

class Var: public KeyName
{
private:
public:
    Var(std::string key, std::string name, std::string note = "");
};

class Vars
{
public:
    Vars();
    Vars(const Vars& other) = delete;
    void add(std::string key, std::string name, std::string note="");
    my_iterator<Var> begin();
    my_iterator<Var> end();
    my_const_iterator<Var> begin() const;
    my_const_iterator<Var> end() const;
    int size() const;

private:
    std::vector<std::unique_ptr<Var> > _vvars;
};

class Plant: public KeyName
{
public:
    Plant();
    Plant(const Plant& other) = delete;
    Plant(std::string key, std::string name, std::string note="", std::string color="");
    void add_var(std::string key, std::string name, std::string note="");
    std::string get_color_str() const;
    void set_color_str(std::string color);
    Vars& get_vars();
    const Vars& get_vars() const;

private:
    std::string color;
    Vars varlist;
};

#endif //PLANT_H
