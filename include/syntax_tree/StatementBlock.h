#ifndef __STATEMENT_BLOCK_H__
#define __STATEMENT_BLOCK_H__

#include "Node.h"

namespace syntax
{
    class StatementBlock : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::StatementBlock;
        }

        void addInstruction(const NodePtr & instruction)
        {
            _instructions.push_back(instruction);
        }

        std::vector<NodePtr> _instructions;
    };
}

#endif // __STATEMENT_BLOCK_H__