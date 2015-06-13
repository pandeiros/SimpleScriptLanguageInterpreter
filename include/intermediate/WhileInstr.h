#ifndef __INTER_WHILE_STATEMENT_H__
#define __INTER_WHILE_STATEMENT_H__

#include <memory>
#include <iostream>

#include "Condition.h"
#include "Block.h"
//#include "intermediate/Literal.h"

namespace inter
{
    class WhileInstr : public Instruction
    {
    public:
        virtual std::shared_ptr<Literal> execute(ScopeInstance * scope,
                                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions)
        {
            while (_condition->execute(scope, functions)->isTruthy())
            {
                auto result = _block->execute(scope, functions);

                if (result && _block->canReturn())
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

        std::shared_ptr<Condition> _condition;
        std::shared_ptr<Block> _block;
    };
}

#endif
