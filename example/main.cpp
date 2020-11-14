#include "ArgsParser.h"

#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    ArgsParser argsParser(argv[0], "Test app");

    const auto intOption = argsParser.addOption(
        "i", "ii", "value", "i option", 33);
    
    const auto stringOption = argsParser.addOption<string>(
        "s", "ss", "string_value", "s option", "XXX");

    const auto boolOption = argsParser.addBooleanOption("b", "bb", "flag");

    const auto intArgument = argsParser.addArgument(
        "int_arg", "int arg", 168, false);

    const auto stringArgument = argsParser.addArgument<string>(
        "string_arg", "string arg", "Hello!", true);

    argsParser.parse(argc, argv);

    cout << intOption.value() << endl;
    cout << stringOption.value() << endl;
    cout << boolOption.value() << endl;
    cout << intArgument.value() << endl;
    cout << stringArgument.value() << endl;
  
    return 0;
}
