#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include "Literal.h"

namespace syntax
{
    class Variable : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::Variable;
        }

        void setName(const std::string & name)
        {
            _name = name;
        }

        void setValue(const std::shared_ptr<syntax::Literal> & value)
        {
            _value = value;
        }

        std::string _name;
        std::shared_ptr<Literal> _value;
    };
}

#endif // __VARIABLE_H__