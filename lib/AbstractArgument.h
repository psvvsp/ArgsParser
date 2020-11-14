#pragma once

#include "Common.h"

namespace ap
{

    class AbstractArgument
    {
    public:
        AbstractArgument(
            const string& argumentName,
            const string& annotation,
            const string& defaultValue,
            const bool optional
        )
            : m_argumentName(argumentName)
            , m_annotation(annotation)
            , m_defaultValue(defaultValue)
            , m_optional(optional)
        {}

        virtual ~AbstractArgument() {}

        virtual void convert(const string& arg) = 0;

        const string& argumentName() const { return m_argumentName; }
        const string& annotation() const { return m_annotation; }
        const string& defaultValue() const { return m_defaultValue; }
        bool optional() const { return m_optional; }

    private:
        const string m_argumentName;
        const string m_annotation;
        const string m_defaultValue;
        bool m_optional;
    };

} // namespace ap
