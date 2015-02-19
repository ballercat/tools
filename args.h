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

#ifndef ARGS_HEADER
#define ARGS_HEADER

#include <string>
#include <vector>
#include <utility>
#include <memory>

namespace tools {

class Arg {
    public:

    /// Convert raw arguments into a vector of pairs
    static std::unique_ptr  <
        std::vector<std::pair<std::string,std::string>>
                            >
    parse(const int& argc, char** argv)
    {
        std::vector<std::pair<std::string,std::string>> arguments;

        for(int k = 0; k < argc; ++k) {

            std::string arg(argv[k]);

            if( !arg.length() )
                continue;

            //Here we check for an argument with '='
            if( arg[1] == '-' ) {

                auto pos = arg.find('=');
                if( pos != std::string::npos ) {

                    std::string opt = arg.substr(0, pos);
                    std::string val = arg.substr(pos + 1);

                    arguments.emplace_back(std::make_pair(opt,val));
                    continue;
                }

                arguments.emplace_back(std::make_pair(arg,""));
                continue;
            }

            arguments.emplace_back(std::make_pair(arg,""));
        }

        return std::make_unique< std::vector< std::pair<std::string,std::string>> >(std::move(arguments));
    }
};

}


#endif
