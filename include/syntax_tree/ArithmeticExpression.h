#ifndef __ARITHMETIC_EXPRESSION_H__
#define __ARITHMETIC_EXPRESSION_H__

#include "RValue.h"
#include "TokenType.h"

namespace syntax
{
    class ArithmeticExpression : public RValue
    {
    public:
        virtual Type getType()
        {
            return Node::Type::ArithmeticExpression;
        }

        void addOperand(const NodePtr & operand)
        {
            _operands.push_back(operand);
        }

        void addOperator(const TokenType & operation)
        {
            _operations.push_back(operation);
        }

        std::shared_ptr<syntax::RValue> retrieveRValue()
        {
            if (_operations.size() == 0 &&
                _operands.size() == 1)
            {
                std::shared_ptr<syntax::ArithmeticExpression> expr =
                    std::dynamic_pointer_cast<syntax::ArithmeticExpression>(_operands.at(0));

                if (expr)
                {
                    if (expr->_operations.size() == 0 &&
                        expr->_operands.size() == 1)
                    {
                        return std::dynamic_pointer_cast<syntax::RValue>(expr->_operands.at(0));
                    }
                    else
                        return nullptr;
                }
                else
                    return nullptr;
            }
            else
                return nullptr;
        }

        std::vector<TokenType> _operations;
        std::vector<NodePtr> _operands;
    };
}

#endif // __ARITHMETIC_EXPRESSION_H__