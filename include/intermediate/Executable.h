#ifndef __INTER_EXECUTABLE_H__
#define __INTER_EXECUTABLE_H__

namespace inter
{
    struct Literal;
    struct ScopeInstance;
    struct Function;

    struct Executable
    {
        virtual std::shared_ptr<Literal> execute(ScopeInstance* scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions) = 0;
    };
}

#endif  // __INTER_EXECUTABLE_H__
