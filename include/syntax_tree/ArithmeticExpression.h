#ifndef __ARITHMETIC_EXPRESSION_H__
#define __ARITHMETIC_EXPRESSION_H__

#include "Assignable.h"
#include "TokenType.h"

/*
 *
 */
namespace syntax
{
    class ArithmeticExpression : public Assignable
    {
    public:
        void addOperand(const NodePtr& node)
        {
            this->operands.push_back(node);
        }
        void addOperator(const TokenType & operation)
        {
            this->operations.push_back(operation);
        }
        virtual Type getType()
        {
            return Node::Type::ArithmeticExpression;
        }

        std::vector<TokenType> operations;
        std::vector<NodePtr> operands;
    };
}

#endif // __ARITHMETIC_EXPRESSION_H__