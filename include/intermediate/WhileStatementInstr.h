#ifndef __INTER_WHILE_STATEMENT_H__
#define __INTER_WHILE_STATEMENT_H__

#include <memory>
#include <iostream>

#include "Instruction.h"
#include "Condition.h"
#include "Block.h"
#include "Executable.h"

namespace inter
{
    struct WhileStatement: public Instruction
    {
        std::shared_ptr<Condition> condition;
        std::shared_ptr<Block> block;

        virtual std::shared_ptr<Literal> execute(
            ScopeInst* scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions
        )
        {
            while (this->condition->execute(scope, functions)->isTruthy())
            {
                auto result = this->block->execute(scope, functions);
                if (result && result->loopJump)
                {
                    if (result->isBreak)
                    {
                        break;
                    }
                    continue;
                }
                if (result && this->block->canDoReturn())
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

#endif
