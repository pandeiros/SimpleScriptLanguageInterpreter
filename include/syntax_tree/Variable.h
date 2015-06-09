#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include "Node.h"

/*
 *
 */
namespace syntax
{
    class Variable : public Node
    {
    public:
        void setName(const std::string& name)
        {
            this->variableName = name;
        }

        void setIndexArg(
            const unsigned int& indexNo,
            const std::shared_ptr<Assignable>& argument
            )
        {
            if (indexNo == 1)
            {
                this->indexArg1 = argument;
            }
            else if (indexNo == 2)
            {
                this->indexArg2 = argument;
            }
            else
            {
                // error
            }
        }
        virtual Type getType()
        {
            return Node::Type::Variable;
        }

        std::string variableName;
        std::shared_ptr<Assignable> indexArg1;
        std::shared_ptr<Assignable> indexArg2;
    };
}

#endif // __VARIABLE_H__