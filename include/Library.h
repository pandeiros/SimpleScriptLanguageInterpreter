#ifndef __LIBRARY_H__
#define __LIBRARY_H__

#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <utility>

#include "Literal.h"

struct Library
{
    typedef const std::vector<std::shared_ptr<inter::Literal>> ArgVec;
    typedef std::function<std::shared_ptr<inter::Literal>(ArgVec&)> StdFun;

    static const std::unordered_map<std::string, std::pair<unsigned int, StdFun>>& getStandardFunctions();
    static const StdFun& getStandardFunction(const std::string& name);
    static unsigned int getFunctionParamsCount(const std::string& name);
    static bool hasFunction(const std::string& name);
    static std::shared_ptr<inter::Literal> callFunction(const std::string& name, ArgVec& arguments);

private:
    static std::shared_ptr<inter::Literal> funPrint(ArgVec& arguments);
    static std::shared_ptr<inter::Literal> funPrintEmpty(ArgVec& arguments);
    static std::shared_ptr<inter::Literal> funGenerate(ArgVec& arguments);
    static std::shared_ptr<inter::Literal> funResize(ArgVec& arguments);
    static std::shared_ptr<inter::Literal> funSize(ArgVec& arguments);
    static std::shared_ptr<inter::Literal> funTranspose(ArgVec& arguments);
};

#endif // __LIBRARY_H__
