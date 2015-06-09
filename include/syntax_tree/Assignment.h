#ifndef __ASSIGNMENT_H__
#define __ASSIGNMENT_H__

#include "Node.h"

/*
 *
 */
namespace syntax
{
    class Variable;
    class Assignable;

    class Assignment : public Node
    {
    public:
        void setVariable(const std::shared_ptr<syntax::Variable>& variable)
        {
            this->variable = variable;
        }
        void setValue(const std::shared_ptr<syntax::Assignable>& value)
        {
            this->value = value;
        }

        virtual Type getType()
        {
            return Node::Type::Assignment;
        }

        std::shared_ptr<Variable> variable;
        std::shared_ptr<Assignable> value;
    };
}

#endif // __ASSIGNMENT_H__