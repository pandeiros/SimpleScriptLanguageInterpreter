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

        std::vector<TokenType> _operations;
        std::vector<NodePtr> _operands;
    };
}

#endif // __ARITHMETIC_EXPRESSION_H__