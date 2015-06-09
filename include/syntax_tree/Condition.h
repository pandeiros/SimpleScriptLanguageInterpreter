#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "Node.h"
#include "TokenType.h"

/*
 *
 */
namespace syntax
{
    class Condition : public Node
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
            return Node::Type::Condition;
        }

        bool negated = false;
        TokenType operation = TokenType::Undefined;
        std::vector<NodePtr> operands;
    };
}

#endif // __CONDITION_H__