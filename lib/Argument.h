#pragma once

#include "AbstractArgument.h"
#include "Converter.h"
#include "Exception.h"

namespace ap
{

    template<class T>
    class Argument : public AbstractArgument
    {
    public:
        Argument(
            const string& argumentName,
            const string& annotation,
            const T& defaultValue,
            const bool optional,
            const function<bool(const T&)>& validator
        )
            : AbstractArgument(
                argumentName,
                annotation,
                Converter::toString(defaultValue),
                optional)
            , m_value(defaultValue)
            , m_validator(validator)
        {}

        void convert(const string& arg) override
        {
            if (!Converter::fromString(arg, m_value)) {
                string error = "failed to convert \""
                    + arg + "\" to \"" + typeid(T).name() + "\""
                    + " (argument \"" + argumentName() + "\")";
                throw Exception(error.c_str());
            }
            if (!m_validator(m_value)) {
                string error = "failed to validate \""
                    + Converter::toString(m_value)
                    + "\" (argument \"" + argumentName() + "\")";
                throw Exception(error.c_str());
            }
        }

        const T& value() const { return m_value; }

    private:
        T m_value;
        function<bool(const T&)> m_validator;
    };

} // namespace ap