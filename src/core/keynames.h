
#ifndef KEYNAMES_H
#define KEYNAMES_H

#include <string>

class KeyName
{
public:
    KeyName();
    KeyName(std::string key, std::string name, std::string note);
    std::string get_key() const;
    std::string get_name() const;
    std::string get_note() const;
    void set_note(std::string);
    void set_name(std::string name);
    explicit operator bool() const;
protected:
    std::string key;
    std::string name;
    std::string note;
};

bool operator==(const KeyName&, const KeyName&);

#endif //KEYNAMES_H
