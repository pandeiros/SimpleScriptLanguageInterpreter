#ifndef __LIBRARY_H__
#define __LIBRARY_H__

#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <utility>

#include "Literal.h"

/*
 * Set of predefined library functions, such as print.
 */
struct Library
{
    // Function arguments and function type.
    typedef const std::vector<std::shared_ptr<inter::Literal>> Arguments;
    typedef std::function<std::shared_ptr<inter::Literal>(Arguments &)> LibFunction;


    static const std::unordered_map<std::string, std::pair<unsigned int, LibFunction>> & getStandardFunctions();
    static const LibFunction & getStandardFunction(const std::string & name);
    static unsigned int getFunctionParamsCount(const std::string & name);
    static bool hasFunction(const std::string & name);
    static std::shared_ptr<inter::Literal> callFunction(const std::string& name, Arguments & arguments);

private:
    static std::shared_ptr<inter::Literal> funPrint(Arguments & arguments);
    static std::shared_ptr<inter::Literal> funNewLine(Arguments & arguments);

    static std::unordered_map<std::string, std::pair<unsigned int, Library::LibFunction>> functions;
};

#endif // __LIBRARY_H__
