
#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <memory>

template<typename T>
class my_iterator :public std::iterator<std::input_iterator_tag, T>
{
public:
    my_iterator(typename std::vector<std::unique_ptr<T> >::iterator itr);
    T& operator*();
    T* operator->();
    my_iterator<T>& operator++();
    my_iterator<T> operator++(int);
    my_iterator<T> operator+(int);
    bool equal(const my_iterator<T> &rhs) const;

private:
   typename std::vector<std::unique_ptr<T> >::iterator _itr;
};
template<typename T>
inline bool operator==(my_iterator<T> const& lhs, my_iterator<T> const& rhs)
{
    return lhs.equal(rhs);
}
template<typename T>

inline bool operator!=(my_iterator<T> const& lhs, my_iterator<T> const& rhs)
{
    return !lhs.equal(rhs);
}

template<typename T>
class my_const_iterator :public std::iterator<std::input_iterator_tag, T>
{
public:
    my_const_iterator(typename std::vector<std::unique_ptr<T> >::const_iterator itr);
    const T& operator*() const;
    const T* operator->() const;
    my_const_iterator<T>& operator++();
    my_const_iterator<T> operator++(int);
    my_const_iterator<T> operator+(int);
    bool equal(my_const_iterator const& rhs) const;

private:
   typename std::vector<std::unique_ptr<T> >::const_iterator _itr;
};
template<typename T>
inline bool operator==(my_const_iterator<T> const& lhs, my_const_iterator<T> const& rhs)
{
    return lhs.equal(rhs);
}
template<typename T>
inline bool operator!=(my_const_iterator<T> const& lhs, my_const_iterator<T> const& rhs)
{
    return !lhs.equal(rhs);
}
template<typename T>
inline my_const_iterator<T> operator+(my_const_iterator<T> const& lhs, int const& rhs)
{
    return my_const_iterator<T>(lhs._itr + rhs);
}

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------
template<typename T>
my_iterator<T>::my_iterator(typename std::vector<std::unique_ptr<T> >::iterator itr) :
    _itr(itr)
{
}

template<typename T>
T& my_iterator<T>::operator*()
{
    return *(*_itr);
}

template<typename T>
T* my_iterator<T>::operator->()
{
    return (*_itr).get();
}

template<typename T>
my_iterator<T>& my_iterator<T>::operator++()
{
    ++_itr;
    return *this;
}

template<typename T>
my_iterator<T> my_iterator<T>::operator++(int)
{
    my_iterator<T> result(*this);
    ++_itr;
    return result;
}

template<typename T>
my_iterator<T> my_iterator<T>::operator+(int other)
{
    return my_iterator<T>(_itr + other);
}

template<typename T>
bool my_iterator<T>::equal(const my_iterator<T> &rhs) const
{
    return _itr == rhs._itr;
}

template<typename T>
my_const_iterator<T>::my_const_iterator(typename std::vector<std::unique_ptr<T> >::const_iterator itr) :
    _itr(itr)
{
}

template<typename T>
const T& my_const_iterator<T>::operator*() const
{
    return *(*_itr);
}

template<typename T>
const T* my_const_iterator<T>::operator->() const
{
    return (*_itr).get();
}

template<typename T>
my_const_iterator<T>& my_const_iterator<T>::operator++()
{
    ++_itr;
    return *this;
}

template<typename T>
my_const_iterator<T> my_const_iterator<T>::operator++(int)
{
    my_const_iterator<T> result(*this);
    ++_itr;
    return result;
}

template<typename T>
my_const_iterator<T> my_const_iterator<T>::operator+(int other)
{
    return my_const_iterator<T>(_itr + other);
}

template<typename T>
bool my_const_iterator<T>::equal(const my_const_iterator<T> &rhs) const
{
    return _itr == rhs._itr;
}


#endif //UTILS_H
