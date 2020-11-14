#pragma once

#include <exception>

class Exception : public std::exception
{
public:
    using std::exception::exception;

};
