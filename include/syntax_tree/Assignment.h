#ifndef __ASSIGNMENT_H__
#define __ASSIGNMENT_H__

#include "syntax_tree/Variable.h"
#include "syntax_tree/RValue.h"

namespace syntax
{
    class Assignment : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::Assignment;
        }

        void setVariable(const std::shared_ptr<syntax::Variable> & variable)
        {
            _variable = variable;
        }

        void setValue(const std::shared_ptr<syntax::RValue> & value)
        {
            _value = value;
        }

        std::shared_ptr<Variable> _variable;
        std::shared_ptr<RValue> _value;
    };
}

#endif // __ASSIGNMENT_H__