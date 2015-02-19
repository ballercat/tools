/*
    The MIT License (MIT)

    Copyright (c) 2015 Arthur B

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef TUPLE
#define TUPLE
#include<cstdlib>
#include<type_traits>

namespace types {

//Tuple class
template <class... Ts> struct tuple {};

template <class T, class... Ts>
struct tuple<T, Ts...> : tuple<Ts...>
{
    //ctor
    tuple(T t, Ts... ts) :
        tuple<Ts...>(ts...),
        tail(t)
    {}

    T tail;
};

//Element holder
template<size_t k, typename T> struct element_holder {};

template <class T, class... Ts>
struct element_holder<0, tuple<T, Ts...>>
{
    using type = T;
};

template <size_t k, class T,  class... Ts>
struct element_holder<k, tuple<T, Ts...>>
{
    using type = typename element_holder<k - 1, tuple<Ts...>>::type;
};

//Tuple size
template <class T> struct tuple_sizer;

//Copied from standard basically
template <class... Ts>
struct tuple_sizer<tuple<Ts...>> :
    public std::integral_constant<size_t, sizeof...(Ts)>
{
};

template <class T>
constexpr size_t tuple_size(T t) { return tuple_sizer<T>::value; }

//Get function
template <size_t k, class... Ts>
typename
std::enable_if<k == 0, typename element_holder<0, tuple<Ts...>>::type&>::type
get_from( tuple<Ts...>& t )
{
    return t.tail;
}

template <size_t k, class T, class... Ts>
typename
std::enable_if<k != 0, typename element_holder<k, tuple<T, Ts...>>::type&>::type
get_from( tuple<T, Ts...>& t )
{
    tuple<Ts...>& base = t;
    return get_from<k - 1>(base);
};

} //namespace types

#endif // TUPLE

