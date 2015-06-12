#ifndef __LOGICAL_EXPRESSION_H__
#define __LOGICAL_EXPRESSION_H__

#include "Assignable.h"
#include "TokenType.h"

/*
 *
 */
namespace syntax
{
    class LogicalExpression : public Assignable
    {
    public:
        void addOperand(const NodePtr& node)
        {
            this->operands.push_back(node);
        }
        void setOperator(const TokenType& operation)
        {
            this->operation = operation;
        }
        void setNegated()
        {
            this->negated = true;
        }
        bool isNegated()
        {
            return this->negated;
        }
        NodePtr& getLeftSide()
        {
            return this->operands.at(0);
        }

        virtual Type getType()
        {
            return Node::Type::LogicalExpression;
        }

        bool negated = false;
        TokenType operation = TokenType::Undefined;
        std::vector<NodePtr> operands;
    };
}

#endif // __LOGICAL_EXPRESSION_H__