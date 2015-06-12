#ifndef __CONST_DECLARATION_H__
#define __CONST_DECLARATION_H__

#include "Node.h"

namespace syntax
{
    class ConstDeclaration : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::ConstDeclaration;
        }

        void setName(const std::string & name)
        {
            _name = name;
        }

        void setType(const std::string & type)
        {
           _type = type;
        }

        void setValue(const std::shared_ptr<RValue> & rvalue)
        {
            _value = rvalue;
        }

        std::string _type;
        std::string _name;
        std::shared_ptr<RValue> _value;
    };
}

#endif // __CONST_DECLARATION_H__