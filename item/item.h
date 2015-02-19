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

#ifndef ITEM_H
#define ITEM_H

#include "../basics.h"
#include <type_traits>
#include <new>
#include <string>
#include <vector>
#include <ostream>

namespace types {

/*  capsule is basically a fancry wrapper around strings.
    It will take any numeric or string value and store it.
    Everything gets distilled to a string.
*/
struct Property {
public:
  Property() = default;

  Property(const std::string &p_value) : value(p_value) {}

  // Construct value based of a arithmetic type
  template <typename A, typename = typename std::enable_if<
                            std::is_arithmetic<A>::value>::type>
  Property(A a)
      : value(std::to_string(a)) {}

  // Asign an arithmetic value
  template <typename T, typename = typename std::enable_if<
                            std::is_arithmetic<T>::value>::type>
  Property &operator=(const T &p_value) {
    value = std::to_string(p_value);
    return *this;
  }

  Property &operator=(const std::string &p_value) {
    value = p_value;
    return *this;
  }

  const std::string to_string() const { return value; }

  int to_int() const { return std::stoi(value); }
  long to_long() const { return std::stol(value); }
  float to_float() const { return std::stof(value); }
  double to_double() const { return std::stod(value); }

private:
  std::string value;
};

std::ostream& operator<<(std::ostream& out, const Property& p) {
    out << p.to_string();
    return out;
}

/// \brief An Item class is a collection of properties
/// Limitations:
///     It's not magic. Only uses strings as indexes! Meaning
///     item['0'] = 'text' followed by item[0] = 42
///     will overwrite item['0'] because everything is storred
///     as a text(std::string). Meant to be used as a collection
///     of unique properties built at run-time.
class Item {
public:
  Item() = default;
  ~Item() = default;

public:
  // A way to keep track of indexes
  struct Index {

    Index() = default;
    Index(const std::string &p_name, const size_t &p_index)
        : index(p_index), name(p_name) {}

    size_t index = 0;
    std::string name;
  };

  template <class T> void push(const std::string &p_token, const T &p_value) {
    index.emplace_back(p_token, data.size());
    data.emplace_back(p_value);
  }

  Property &operator[](const std::string &p_token) {
    for (auto &t : index) {
      if (t.name == p_token)
        return data[t.index];
    }

    // Construct a new value
    index.emplace_back(p_token, data.size());
    data.push_back(Property());
    return data[data.size() - 1];
  }
  Property &operator[](const int &p_token) {
    return operator[](std::to_string(p_token));
  }

  const std::vector<Index> &get_indexes() const { return index; }
  const std::vector<Property> &get_data() const { return data; }

private:
  std::vector<Index> index;
  std::vector<Property> data;
};

std::ostream& operator<<(std::ostream& out, const Item& item)
{
    auto index  = item.get_indexes();
    auto data = item.get_data();
    out << "Item {\n";
    for( const auto& i : index ) {
        out << "\t['" << i.name << "'] = " << data[i.index].to_string() << "\n";
    }
    out << "}";

    return out;
}

} //namespace types

#endif //ITEM_H
