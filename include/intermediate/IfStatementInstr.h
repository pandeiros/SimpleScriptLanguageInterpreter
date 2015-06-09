#ifndef __INTER_IF_STATEMENT_INSTR_H__
#define __INTER_IF_STATEMENT_INSTR_H__

#include <memory>
#include <iostream>

#include "Instruction.h"
#include "Condition.h"
#include "Block.h"
#include "Executable.h"

namespace inter
{
    struct IfStatementInstr : public Instruction
    {
        std::shared_ptr<Condition> condition;
        std::shared_ptr<Block> trueBlock;
        std::shared_ptr<Block> falseBlock;

        virtual std::shared_ptr<Literal> execute(
            ScopeInst* scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions
        )
        {
            if (this->condition->execute(scope, functions)->isTruthy())
            {
                return this->trueBlock->execute(scope, functions);
            }
            else if (this->falseBlock)
            {
                return this->falseBlock->execute(scope, functions);
            }

            return nullptr;
        }

        virtual bool canDoReturn()
        {
            return true;
        }
    };
}

#endif // __INTER_IF_STATEMENT_INSTR_H__
