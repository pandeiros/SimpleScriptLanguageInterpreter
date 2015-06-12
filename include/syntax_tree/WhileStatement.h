#ifndef __WHILE_STATEMENT_H__
#define __WHILE_STATEMENT_H__

#include "Node.h"

/*
 *
 */
namespace syntax
{
    class WhileStatement : public Node
    {
    public:
        void setCondition(const std::shared_ptr<LogicalExpression>& conditionNode)
        {
            this->conditionNode = conditionNode;
        }
        void setBlock(const std::shared_ptr<StatementBlock>& blockNode)
        {
            this->blockNode = blockNode;
        }
        virtual Type getType()
        {
            return Node::Type::WhileStatement;
        }

        std::shared_ptr<LogicalExpression> conditionNode;
        std::shared_ptr<StatementBlock> blockNode;
    };
}

#endif // __WHILE_STATEMENT_H__