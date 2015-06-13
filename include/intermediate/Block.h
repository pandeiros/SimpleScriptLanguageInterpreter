#ifndef __INTER_BLOCK_H__
#define __INTER_BLOCK_H__

#include <vector>
#include <memory>
#include <iostream>

#include "Executable.h"
#include "ScopePrototype.h"
#include "intermediate/Literal.h"
#include "Instruction.h"

namespace inter
{
    class Block: public Instruction
    {
    public:
        virtual std::shared_ptr<Literal> execute(ScopeInstance * scope,
                                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions)
        {
            auto thisScope = _scopePrototype.instantiate(scope);

            for(auto & instruction : _instructions)
            {
                auto result = instruction->execute(&thisScope, functions);
                if (result && instruction->canReturn())
                {
                    return result;
                }
            }

            return nullptr;
        }

        virtual bool canReturn()
        {
            return true;
        }

        ScopePrototype _scopePrototype;
        std::vector<std::shared_ptr<Instruction>> _instructions;
    };
}

#endif // __INTER_BLOCK_H__
