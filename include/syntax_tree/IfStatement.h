#ifndef __IF_STATEMENT_H__
#define __IF_STATEMENT_H__

#include "LogicalExpression.h"

namespace syntax
{
    class IfStatement : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::IfStatement;
        }

        void setCondition(const std::shared_ptr<LogicalExpression> & condition)
        {
            _condition = condition;
        }

        void setTrueBlock(const std::shared_ptr<StatementBlock> & trueBlock)
        {
            _trueBlock = trueBlock;
        }

        void setFalseBlock(const std::shared_ptr<StatementBlock> & falseBlock)
        {
            _falseBlock = falseBlock;
        }

        std::shared_ptr<LogicalExpression> _condition;
        std::shared_ptr<StatementBlock> _trueBlock;
        std::shared_ptr<StatementBlock> _falseBlock;
    };
}

#endif // __IF_STATEMENT_H__