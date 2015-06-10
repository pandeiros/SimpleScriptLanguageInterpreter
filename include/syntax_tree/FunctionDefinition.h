#ifndef __FUNCTION_DEFINITION_H__
#define __FUNCTION_DEFINITION_H__

#include "Node.h"

namespace syntax
{
    class StatementBlock;

    class FunctionDefinition : public Node
    {
    public:
        void setName(const std::string& name)
        {
            this->name = name;
        }
        void setParameters(const std::vector<std::string>& parameters)
        {
            this->parameters = parameters;
        }
        void setBlock(const std::shared_ptr<StatementBlock>& blockNode)
        {
            this->blockNode = blockNode;
        }
        virtual Type getType()
        {
            return Node::Type::FunctionDefinition;
        }

        std::string name;
        std::vector<std::string> parameters;
        std::shared_ptr<StatementBlock> blockNode;
    };
}

#endif // __FUNCTION_DEFINITION_H__