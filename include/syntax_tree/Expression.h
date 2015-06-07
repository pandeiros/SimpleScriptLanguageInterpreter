#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include "Node.h"

/*
 *
 */
class Expression : public Assignable
{
public:
    void addOperand (const NodePtr& node)
    {
        this->operands.push_back (node);
    }
    void addOperator (const TokenType& operation)
    {
        this->operations.push_back (operation);
    }
    virtual Type getType ()
    {
        return Node::Type::Expression;
    }

    std::vector<TokenType> operations;
    std::vector<NodePtr> operands;
};

#endif // __EXPRESSION_H__