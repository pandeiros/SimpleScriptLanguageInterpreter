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
            {"print", {1, Library::LibFunction(&Library::funPrint)}},
            {"nl", {0, Library::LibFunction(&Library::funNewLine)}}
        };

        functions = list;
    }

    return functions;
}

const Library::LibFunction&
Library::getStandardFunction(const std::string& name)
{
    return (Library::getStandardFunctions()).at(name).second;
}

bool
Library::hasFunction(const std::string& name)
{
    return (Library::getStandardFunctions()).count(name) == 1;
}

unsigned int
Library::getFunctionParamsCount(const std::string& name)
{
    return (Library::getStandardFunctions()).at(name).first;
}

std::shared_ptr<inter::Literal>
Library::callFunction(const std::string& name, Arguments& arguments)
{
    return ((Library::getStandardFunction(name))(arguments));
}

std::shared_ptr<inter::Literal> Library::funPrint(Arguments & arguments)
{
    // TODO change to normal, non-matrix print.
    for (auto & arg : arguments.at(0)->data)
    {
        for (auto & rowIt : arg)
        {
            std::cout << rowIt << " ";
        }
        std::cout << std::endl;
    }

    auto result = std::make_shared<inter::Literal>();
    result->data = {{1}};
    return result;
}

std::shared_ptr<inter::Literal> Library::funNewLine(Arguments & arguments)
{
    std::cout << std::endl;

    auto result = std::make_shared<inter::Literal>();
    result->data = {{1}};
    return result;
}
