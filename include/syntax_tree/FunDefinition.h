#ifndef __FUN_DEFINITION_H__
#define __FUN_DEFINITION_H__

#include "Node.h"

class StatementBlock;

/*
 *
 */
class FunDefinition : public Node
{
public:
    void setName (const std::string& name)
    {
        this->name = name;
    }
    void setParameters (const std::vector<std::string>& parameters)
    {
        this->parameters = parameters;
    }
    void setBlock (const std::shared_ptr<StatementBlock>& blockNode)
    {
        this->blockNode = blockNode;
    }
    virtual Type getType ()
    {
        return Node::Type::FunDefinition;
    }

    std::string name;
    std::vector<std::string> parameters;
    std::shared_ptr<StatementBlock> blockNode;
};

#endif // __FUN_DEFINITION_H__