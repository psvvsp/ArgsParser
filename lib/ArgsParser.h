#pragma once

#include "Argument.h"
#include "Common.h"
#include "Option.h"

#include <iostream>

class ArgsParser
{
public:
	
	template<class T>
	class Option
	{
	public:
		const T &value() const { return m_option->value(); }
	private:
		::Option<T> *m_option;
		friend class ArgsParser;
	};

	template<class T>
	class Argument
	{
	public:
		const T& value() const { return m_argument->value(); }
	private:
		::Argument<T> *m_argument;
		friend class ArgsParser;
	};

	ArgsParser(const string& path, const string& annotation);

	template<class T>
	Option<T> addOption(
		const string& shortOption,
		const string& longOption,
		const string& argumentName,
		const string& annotation,
		const T& defaultValue,
		const function<bool(const T&)>& validator = [](const T&) { return true; }
	)
	{
		Option<T> result;
		result.m_option = new ::Option<T>(
			shortOption,
			longOption,
			argumentName,
			annotation,
			defaultValue,
			validator);

		m_options.emplace_back(result.m_option);

		return result;
	}

	Option<bool> addBooleanOption(
		const string& shortOption,
		const string& longOption,
		const string& annotation
	)
	{
		Option<bool> result;
		result.m_option = new ::Option<bool>(
			shortOption,
			longOption,
			annotation);

		m_options.emplace_back(result.m_option);

		return result;
	}

	template<class T>
	Argument<T> addArgument(
		const string& argumentName,
		const string& annotation,
		const T& defaultValue,
		const bool optional,
		const function<bool(const T&)>& validator
		= [](const T&) { return true; }
	)
	{
		if (!optional && !m_arguments.empty() &&
			m_arguments.back()->optional()) {
				std::cerr
					<< "ArgsParser error: trying to add"
					<< "a positional argument \"" << argumentName
					<< "\" after an optional one" << std::endl;
			exit(1);
		}

		Argument<T> result;
		result.m_argument = new ::Argument<T>(
			argumentName,
			annotation,
			defaultValue,
			optional,
			validator);

		m_arguments.emplace_back(result.m_argument);

		return result;
	}

	void parse(int argc, char** argv);

private:
	ArgsParser(const ArgsParser&) = delete;
	ArgsParser& operator = (const ArgsParser&) = delete;

	string helpString() const;

	Option<bool> m_helpOption;

	string m_path;
	string m_annotation;

	vector<unique_ptr<AbstractOption>> m_options;
	vector<unique_ptr<AbstractArgument>> m_arguments;
};
