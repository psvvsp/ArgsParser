#pragma once

#include "Common.h"

class AbstractOption
{
public:
	AbstractOption(
		const string& shortOption,
		const string& longOption,
		const string& argumentName,
		const string& annotation,
		const string& defaultValue)
		: m_shortOption(shortOption)
		, m_longOption(longOption)
		, m_argumentName(argumentName)
		, m_annotation(annotation)
		, m_defaultValue(defaultValue)
	{}

	virtual ~AbstractOption() {}

	virtual int argsCount() const = 0;
	virtual bool parse(const vector<string>& args) = 0;

	const string& shortOption() const { return m_shortOption; }
	const string& longOption() const { return m_longOption; }
	const string& argumentName() const { return m_argumentName; }
	const string& annotation() const { return m_annotation; }
	const string& defaultValue() const { return m_defaultValue; }

private:
	const string m_shortOption;
	const string m_longOption;
	const string m_argumentName;
	const string m_annotation;
	const string m_defaultValue;
};

