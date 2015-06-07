#ifndef __IF_STATEMENT_H__
#define __IF_STATEMENT_H__

#include "Node.h"

class StatementBlock;

/*
 *
 */
class IfStatement : public Node
{
public:
    void setCondition (const std::shared_ptr<Condition>& conditionNode)
    {
        this->conditionNode = conditionNode;
    }
    void setTrueBlock (const std::shared_ptr<StatementBlock>& trueBlockNode)
    {
        this->trueBlockNode = trueBlockNode;
    }
    void setFalseBlock (const std::shared_ptr<StatementBlock>& falseBlockNode)
    {
        this->falseBlockNode = falseBlockNode;
    }
    virtual Type getType ()
    {
        return Node::Type::IfStatement;
    }

    std::shared_ptr<Condition> conditionNode;
    std::shared_ptr<StatementBlock> trueBlockNode;
    std::shared_ptr<StatementBlock> falseBlockNode;
};

#endif // __IF_STATEMENT_H__