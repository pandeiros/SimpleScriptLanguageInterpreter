#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "FunctionDefinition.h"

namespace syntax
{
    class Program : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::Program;
        }

        void addFunction(const std::shared_ptr<FunctionDefinition> & function)
        {
            _functions.push_back(function);
        }


        std::vector<std::shared_ptr<FunctionDefinition>> _functions = {};
    };
}

#endif // __PROGRAM_H__