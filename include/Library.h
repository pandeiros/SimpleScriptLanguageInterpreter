#ifndef __LIBRARY_H__
#define __LIBRARY_H__

#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <utility>

#include "intermediate/Literal.h"

/*
 * Set of predefined library functions, such as print.
 */
class Library
{
public:
    // Function arguments and function type.
    typedef const std::vector<std::shared_ptr<inter::Literal>> Arguments;
    typedef std::function<std::shared_ptr<inter::Literal>(Arguments &)> LibFunction;

    // Function gettters.
    static const std::unordered_map<std::string, std::pair<unsigned int, LibFunction>> & getStandardFunctions();
    static const LibFunction & getStandardFunction(const std::string & name);
    static unsigned int getFunctionParamsCount(const std::string & name);
    static bool hasFunction(const std::string & name);

    // Call method.
    static std::shared_ptr<inter::Literal> callFunction(const std::string& name, Arguments & arguments);

private:
    // Defined standard library functions.
    static std::shared_ptr<inter::Literal> functionPrint(Arguments & arguments);
    static std::shared_ptr<inter::Literal> functionNewLine(Arguments & arguments);

    // Function container with argument count and functor.
    static std::unordered_map<std::string, std::pair<unsigned int, Library::LibFunction>> functions;
};

#endif // __LIBRARY_H__
