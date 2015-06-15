#include "Library.h"

#include <iostream>

#include <limits>
#include "MessageHandler.h"

// Static init.
std::unordered_map<std::string, std::pair<unsigned int, Library::LibFunction>> Library::functions =
    std::unordered_map<std::string, std::pair<unsigned int, Library::LibFunction>>();

const std::unordered_map<std::string, std::pair<unsigned int, Library::LibFunction>> & Library::getStandardFunctions()
{
    if (functions.size() == 0)
    {
        static const std::unordered_map<std::string, std::pair<unsigned int, Library::LibFunction>> list = {
            {"print", {1, Library::LibFunction(&Library::functionPrint)}},
            {"println", {1, Library::LibFunction(&Library::functionPrintln)}},
            {"nl", {0, Library::LibFunction(&Library::functionNewLine)}}
        };

        functions = list;
    }

    return functions;
}

const Library::LibFunction & Library::getStandardFunction(const std::string & name)
{
    return (Library::getStandardFunctions()).at(name).second;
}

bool Library::hasFunction(const std::string & name)
{
    return (Library::getStandardFunctions()).count(name) == 1;
}

unsigned int Library::getFunctionParamsCount(const std::string & name)
{
    return (Library::getStandardFunctions()).at(name).first;
}

std::shared_ptr<inter::Literal> Library::callFunction(const std::string & name, Arguments & arguments)
{
    return ((Library::getStandardFunction(name))(arguments));
}

std::shared_ptr<inter::Literal> Library::functionPrint(Arguments & arguments)
{
    if (arguments.size() == 0)
        return nullptr;

    for (auto & arg : arguments)
    {
        if (arg->_type == "bool")
            std::cout << std::boolalpha << arg->_boolValue;
        else if (arg->_type == "int")
            std::cout << arg->_intValue;
        else if (arg->_type == "float")
            std::cout << arg->_floatValue;
        else if (arg->_type == "string")
            std::cout << arg->_stringValue;
        else
            std::cout << "Unknown";
    }

    return arguments.at(0);
}

std::shared_ptr<inter::Literal> Library::functionPrintln(Arguments & arguments)
{
    Library::functionPrint(arguments);

    std::cout << "\n";

    return arguments.at(0);
}

std::shared_ptr<inter::Literal> Library::functionNewLine(Arguments & arguments)
{
    std::cout << std::endl;

    return std::make_shared<inter::Literal>();
}
