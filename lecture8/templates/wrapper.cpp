// Trying to put templates in separate files like normal classes doesn't work, since C++ needs to specialize all the template code when you use a class template.
// Putting the implementation in a file separate from the header means the compiler will only specialize the code in the header, and the implementations of the member functions will be nowhere to be found.

#include "wrapper.hpp"

template <typename T>
wrapper<T>::wrapper(T t) : t {t}
{}

template <typename T>
T wrapper<T>::get() const
{
    return t;
}

template <typename T>
void wrapper<T>::set(T t)
{
    this.t = t;
}
