#ifndef __WHILE_STATEMENT_H__
#define __WHILE_STATEMENT_H__

#include "Node.h"

namespace syntax
{
    class WhileStatement : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::WhileStatement;
        }

        void setCondition(const std::shared_ptr<LogicalExpression> & condition)
        {
            _condition = condition;
        }

        void setBlock(const std::shared_ptr<StatementBlock>& block)
        {
            _block = block;
        }

        std::shared_ptr<LogicalExpression> _condition;
        std::shared_ptr<StatementBlock> _block;
    };
}

#endif // __WHILE_STATEMENT_H__