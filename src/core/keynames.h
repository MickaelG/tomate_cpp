
#ifndef KEYNAMES_H
#define KEYNAMES_H

#include <string>
#include <list>
using namespace std;

class KeyName
{
    protected:
        string key;
        string name;
        string note;
    public:
        KeyName();
        KeyName(string key, string name, string note);
        string get_key() const;
        string get_name() const;
        string get_note() const;
        void set_note(string);
        void set_name(string name);
        explicit operator bool() const;
};
extern KeyName NullKeyName;

bool operator==(const KeyName&, const KeyName&);

/*
//this is dangerous to store reference from vector elements. use list.
template <class T> class ListKeyNames: public list<T>
{
public:
    T& index(int data_index);
};
*/

class KeyNames: public list<KeyName>
{
public:
    KeyName& index(int data_index);
};

#endif //KEYNAMES_H
