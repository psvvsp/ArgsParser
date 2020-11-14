#pragma once

#include "AbstractOption.h"
#include "Common.h"
#include "Converter.h"
#include "Exception.h"

template<class T>
class Option : public AbstractOption
{
public:
	Option(
		const string& shortOption,
		const string& longOption,
		const string& argumentName,
		const string& annotation,
		const T& defaultValue,
		const function<bool(const T&)>& validator
	)
		: AbstractOption(
			shortOption,
			longOption,
			argumentName,
			annotation,
			Converter::toString(defaultValue))
		, m_value(defaultValue)
		, m_validator(validator)
	{}

	int argsCount() const override { return 2; }

	bool parse(const vector<string>& args) override {
		if (args.size() != 2)
			return false;
		if ("-" + shortOption() != args[0] &&
			"--" + longOption() != args[0])
			return false;
		if (!Converter::fromString(args[1], m_value)) {
			string error = "failed to convert \""
					+ args[1] + "\" to \"" + typeid(T).name() + "\""
					+ " (option \"" + args[0] + "\")";
			throw Exception(error.c_str());
		}
		if (!m_validator(m_value)) {
			string error = "failed to validate \""
				+ Converter::toString(m_value)
				+ "\" (option \"" + args[0] + "\")";
			throw Exception(error.c_str());
		}
		return true;
	}

	const T& value() const { return m_value; }

private:
	T m_value;
	function<bool(const T&)> m_validator;
};

template<>
class Option<bool> : public AbstractOption
{
public:
	Option(
		const string& shortOption,
		const string& longOption,
		const string& annotation
	)
		: AbstractOption(
			shortOption,
			longOption,
			"",
			annotation,
			"")
		, m_value(false)
	{}

	int argsCount() const override { return 1; }
	bool parse(const vector<string>& args) override
	{
		if (args.size() != 1)
			return false;
		if ("-" + shortOption() != args[0] &&
			"--" + longOption() != args[0])
			return false;
		m_value = true;
		return true;
	}

	const bool &value() const { return m_value; }

private:
	bool m_value;
};
