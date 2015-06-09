#ifndef __CALL_H__
#define __CALL_H__

#include "Node.h"
#include <vector>

/*
 *
 */
namespace syntax
{
    class Call : public Assignable
    {
    public:
        void setName(const std::string& name)
        {
            this->name = name;
        }
        void addArgument(const std::shared_ptr<Assignable>& assignableNode)
        {
            this->arguments.push_back(assignableNode);
        }
        virtual Type getType()
        {
            return Node::Type::Call;
        }

        std::string name;
        std::vector<std::shared_ptr<Assignable>> arguments;
    };
}

#endif // __CALL_H__