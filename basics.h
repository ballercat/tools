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

#ifndef BASICS_H
#define BASICS_H

#ifndef TRUE
    #define TRUE    1
#endif

#ifndef FALSE
    #define FALSE   0
#endif

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#elif defined(__cplusplus)
# define UNUSED(x)
#else
# define UNUSED(x) x
#endif

//some includes
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>

///
/// \info std::map print operator
///
template<typename T, typename U>
std::ostream& operator<<(std::ostream& out, std::unordered_map<T,U>& p) {
    out << "{";
    for( const auto& v : p ) {
        out << "\n\t[" << v.first << "]\t=\t" << v.second;
    }
    out << "\n}";
    return out;
}

template<typename T, typename U>
std::ostream& operator<<(std::ostream& out, std::vector<std::pair<T,U>>& v) {
    out << "{";
    for( const auto& p : v )
        out << "\n\t[" << p.first << "]\t=\t" << p.second;
    out << "\n}";
    return out;
}

namespace tools {

class Path {
    public:

    static std::string trim(const std::string& path) {

        std::string trimmed = path;
        std::string::size_type pos = trimmed.find('/');

        while( pos != std::string::npos ) {

            trimmed = trimmed.substr(pos);
            pos = trimmed.find('/');
        }

        return trimmed;
    }
};

}

#endif //BASICS_H
