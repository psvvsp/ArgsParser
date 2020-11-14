#include "ArgsParser.h"
#include <cassert>
using namespace ap;

ArgsParser::ArgsParser(const string& path, const string& annotation)
    : m_path(path), m_annotation(annotation)
{
    m_helpOption = addBooleanOption(
        "h", "help", "prints this message and quits");
}

void ArgsParser::parse(int argc, char** argv)
{
    int i = 1;
    while (i < argc) {

        bool match = false;
        for (auto& option : m_options) {

            const int argsCount = option->argsCount();
            if (i + argsCount > argc)
                continue;

            vector<string> args;
            for (int j = 0; j < option->argsCount(); j++)
                args.push_back(argv[i + j]);

            try {
                match = option->parse(args);
            }
            catch (const Exception& e) {
                std::cerr << "ArgsParser error: "
                    << e.what() << std::endl;
                std::cout << helpString();
                exit(1);
            }

            if (match) {
                i += argsCount;
                break;
            }
        }

        if (!match)
            break;
    }

    if (m_helpOption.value()) {
        std::cout << helpString();
        exit(0);
    }

    int j = 0;
    const int size = static_cast<int>(m_arguments.size());
    for (; (i + j < argc) && (i + j < size); j++) {
        try {
            m_arguments[j]->convert(argv[i + j]);
        }
        catch (const Exception& e) {
            std::cerr << "ArgsParser error: " << e.what() << std::endl;
            std::cout << helpString();
            exit(1);
        }
    }

    if (i + j < argc) {
        std::cerr << "ArgsParser error: too many positional arguments"
            << std::endl;
        std::cout << helpString();
        exit(1);
    }

    if (j < size && !m_arguments[j]->optional()) {
        std::cerr << "ArgsParser error: too few positional arguments"
            << std::endl;
        std::cout << helpString();
        exit(1);
    }
}

string ArgsParser::helpString() const
{
    string result = "\nUsage:\n";
    result += "\t" + m_path + " [<Options>]";

    for (const auto& argument : m_arguments) {
        result += " ";
        if (argument->optional()) result += "[";
        result += "<" + argument->argumentName() + ">";
        if (argument->optional()) result += "]";
    }

    result += "\n";
    result += "\nDescription:\n";
    result += "\t" + m_annotation + "\n";

    vector<string> leftParts, rightParts;

    size_t maxLength = 0;
    for (const auto& option : m_options) {
        
        string leftPart, rightPart;

        if (option->argsCount() == 1) {
            leftPart
                = string("-") + option->shortOption() + ", "
                + string("--") + option->longOption();
            rightPart = option->annotation();
        }
        else if (option->argsCount() == 2) {
            leftPart
                = string("-") + option->shortOption() + ", "
                + string("--") + option->longOption() + " <"
                + option->argumentName() + ">";
            rightPart
                = option->annotation() + ", "
                + "default value: \"" + option->defaultValue() + "\"";
        }
        else
            assert(false);

        if (leftPart.size() > maxLength)
            maxLength = leftPart.size();

        leftParts.push_back(leftPart);
        rightParts.push_back(rightPart);
    }

    for (const auto& argument : m_arguments) {

        string leftPart, rightPart;

        if (argument->optional()) {
            leftPart = argument->argumentName();
            rightPart
                = argument->annotation() + ", "
                + "default value: \"" + argument->defaultValue() + "\"";
        }
        else {
            leftPart = argument->argumentName();
            rightPart = argument->annotation();
        }

        if (leftPart.size() > maxLength)
            maxLength = leftPart.size();

        leftParts.push_back(leftPart);
        rightParts.push_back(rightPart);
    }

    for (string& leftPart : leftParts) {
        leftPart.resize(maxLength, ' ');
    }

    result += "\nOptions:\n";

    for (size_t i = 0; i < m_options.size(); i++) {
        result += "\t" + leftParts[i] + "\t" + rightParts[i] + "\n";
    }

    result += "\nArguments:\n";

    for (size_t i = m_options.size(); i < leftParts.size(); i++) {
        result += "\t" + leftParts[i] + "\t" + rightParts[i] + "\n";
    }

    return result;
}
