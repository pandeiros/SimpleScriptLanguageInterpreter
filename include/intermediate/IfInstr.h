#ifndef __INTER_IF_INSTR_H__
#define __INTER_IF_INSTR_H__

#include <memory>
#include <iostream>

#include "LogicalExpression.h"
#include "Block.h"

namespace inter
{
    class IfInstr : public Instruction
    {
    public:
        std::shared_ptr<LogicalExpression> condition;
        std::shared_ptr<Block> trueBlock;
        std::shared_ptr<Block> falseBlock;

        virtual std::shared_ptr<Literal> execute(
            ScopeInstance* scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions
        )
        {
            if (this->condition->execute(scope, functions)->isEqualToTrue())
            {
                return this->trueBlock->execute(scope, functions);
            }
            else if (this->falseBlock)
            {
                return this->falseBlock->execute(scope, functions);
            }

            return nullptr;
        }

        virtual bool canReturn()
        {
            return true;
        }
    };
}

#endif // __INTER_IF_INSTR_H__
