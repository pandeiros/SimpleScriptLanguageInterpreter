#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "Node.h"

class FunctionDefinition;

namespace syntax
{
    class Program : public Node
    {
    public:
        void addFunction(const std::shared_ptr<FunctionDefinition>& function)
        {
            this->functions.push_back(function);
        }
        virtual Type getType()
        {
            return Node::Type::Program;
        }

        std::vector<std::shared_ptr<FunctionDefinition>> functions = {};
    };
}

#endif // __PROGRAM_H__