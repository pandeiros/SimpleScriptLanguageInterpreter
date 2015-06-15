#ifndef __INTER_EXPRESSION_H__
#define __INTER_EXPRESSION_H__

#include <vector>
#include <memory>
#include <iostream>

#include "Assignable.h"
#include "ArithmeticOperand.h"
#include "TokenType.h"

namespace inter
{
    struct ArithmeticExpression : public ArithmeticOperand, public LogicalOperand
    {
        std::vector<TokenType> operations;
        std::vector<std::shared_ptr<ArithmeticOperand>> operands;

        virtual std::shared_ptr<Literal> execute(
            ScopeInstance* scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions
        )
        {
            if (this->operations.size() == 0)
            {
                return this->operands.at(0)->execute(scope, functions);
            }

            auto result = this->operands.at(0)->execute(scope, functions);
            unsigned int i = 0;
            for(auto & op: this->operations)
            {
                auto & operand = this->operands.at(i + 1);
                i++;

                if (op == TokenType::Plus)
                {
                    *result += *(operand->execute(scope, functions));
                }
                else if (op == TokenType::Minus)
                {
                    *result -= *(operand->execute(scope, functions));
                }
                else if (op == TokenType::Multiply)
                {
                    *result *= *(operand->execute(scope, functions));
                }
                else if (op == TokenType::Divide)
                {
                    *result /= *(operand->execute(scope, functions));
                }
                else if (op == TokenType::Modulo)
                {
                    *result %= *(operand->execute(scope, functions));
                }
                else
                {
                    MessageHandler::error(std::string("Invalid arithmetical expression operator!"));
                    return nullptr;
                }
            }

            return result;
        }
    };
}

#endif // __INTER_EXPRESSION_H__
