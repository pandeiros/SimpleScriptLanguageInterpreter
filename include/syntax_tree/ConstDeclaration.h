#ifndef __CONST_DECLARATION_H__
#define __CONST_DECLARATION_H__

#include "Node.h"

/*
 *
 */
namespace syntax
{
    class ConstDeclaration : public Node
    {
    public:
        void setName(const std::string & name)
        {
            this->name = name;
        }

        void setType(const std::string & type)
        {
            this->type = type;
        }

        void setValue(const std::shared_ptr<Assignable> & assignableNode)
        {
            this->assignableNode = assignableNode;
        }

        virtual Type getType()
        {
            return Node::Type::ConstDeclaration;
        }

        std::string type;
        std::string name;
        std::shared_ptr<Assignable> assignableNode;
    };
}

#endif // __CONST_DECLARATION_H__