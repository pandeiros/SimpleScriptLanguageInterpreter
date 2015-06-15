#ifndef __INTER_EXECUTABLE_H__
#define __INTER_EXECUTABLE_H__

#include <unordered_map>

namespace inter
{
    class Literal;
    class ScopeInstance;
    class Function;

    struct Executable
    {
        virtual std::shared_ptr<Literal> execute(ScopeInstance* scope,
                                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions) = 0;
    };
}

#endif  // __INTER_EXECUTABLE_H__
