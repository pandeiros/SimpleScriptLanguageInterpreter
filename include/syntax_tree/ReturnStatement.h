#ifndef __RETURN_STATEMENT_H__
#define __RETURN_STATEMENT_H__

#include "Node.h"

/*
 *
 */
class ReturnStatement : public Node
{
public:
    void setValue (const std::shared_ptr<Assignable>& assignableNode)
    {
        this->assignableNode = assignableNode;
    }
    virtual Type getType ()
    {
        return Node::Type::ReturnStatement;
    }

    std::shared_ptr<Assignable> assignableNode;
};

#endif // __RETURN_STATEMENT_H__