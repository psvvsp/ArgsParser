#pragma once

#include "Common.h"

#include <sstream>

namespace ap
{

    class Converter
    {
    public:
        template<class T>
        static string toString(const T& t)
        {
            std::ostringstream out;
            out << t;
            return out.str();
        }

        template<class T>
        static bool fromString(const string& str, T& t)
        {
            std::istringstream in(str);
            in >> t;
            return !!in;
        }
    };

} // namespace ap