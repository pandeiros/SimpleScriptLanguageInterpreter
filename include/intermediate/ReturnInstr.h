#ifndef __INTER_RETURN_INSTR_H__
#define __INTER_RETURN_INSTR_H__

#include <memory>

#include "Instruction.h"
#include "Assignable.h"
#include "Executable.h"

namespace inter
{
    struct ReturnInstr: public Instruction
    {
        std::shared_ptr<Assignable> value;

        virtual std::shared_ptr<Literal> execute(
            ScopeInstance* scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions
        )
        {
            return this->value->execute(scope, functions);
        }

        virtual bool canDoReturn()
        {
            return true;
        }
    };
}

#endif // __INTER_RETURN_INSTR_H__
