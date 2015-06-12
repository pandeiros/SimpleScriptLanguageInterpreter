#ifndef __LOGICAL_EXPRESSION_H__
#define __LOGICAL_EXPRESSION_H__

#include "RValue.h"
#include "TokenType.h"

namespace syntax
{
    class LogicalExpression : public RValue
    {
    public:
        virtual Type getType()
        {
            return Node::Type::LogicalExpression;
        }

        void addOperand(const NodePtr & operand)
        {
            _operands.push_back(operand);
        }

        void setOperator(const TokenType & operation)
        {
            _operation = operation;
        }

        void setNegated()
        {
            _isNegated = true;
        }

        bool isNegated()
        {
            return _isNegated;
        }

        NodePtr & getLeftSide()
        {
            return _operands.at(0);
        }

        bool _isNegated = false;
        TokenType _operation = TokenType::Undefined;
        std::vector<NodePtr> _operands;
    };
}

#endif // __LOGICAL_EXPRESSION_H__