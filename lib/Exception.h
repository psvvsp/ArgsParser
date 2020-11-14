#pragma once

#include <exception>

namespace ap
{

    class Exception : public std::exception
    {
    public:
        using std::exception::exception;

    };

} // namespace ap
