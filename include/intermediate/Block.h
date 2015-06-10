#ifndef __INTER_BLOCK_H__
#define __INTER_BLOCK_H__

#include <vector>
#include <memory>
#include <iostream>

#include "Executable.h"
#include "ScopePrototype.h"
#include "Instruction.h"
#include "Literal.h"

namespace inter
{
    struct Block: public Instruction
    {
        ScopePrototype scopePrototype;
        std::vector<std::shared_ptr<Instruction>> instructions;

        virtual std::shared_ptr<Literal> execute(
            ScopeInstance * scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions
        )
        {
            auto thisScope = this->scopePrototype.instantiate(scope);

            for(auto& it: this->instructions)
            {
                auto result = it->execute(&thisScope, functions);
                if (result && ( result->loopJump || it->canDoReturn()))
                {
                    return result;
                }
            }

            return nullptr;
        }

        virtual bool canDoReturn()
        {
            return true;
        }
    };
}

#endif // __INTER_BLOCK_H__
