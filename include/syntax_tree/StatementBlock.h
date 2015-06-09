#ifndef __STATEMENT_BLOCK_H__
#define __STATEMENT_BLOCK_H__

#include "Node.h"

/*
 *
 */
namespace syntax
{
    class StatementBlock : public Node
    {
    public:
        void addInstruction(const NodePtr& node)
        {
            this->instructions.push_back(node);
        }
        virtual Type getType()
        {
            return Node::Type::StatementBlock;
        }

        std::vector<NodePtr> instructions;
    };
}

#endif // __STATEMENT_BLOCK_H__