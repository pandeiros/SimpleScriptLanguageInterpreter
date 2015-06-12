#ifndef __CALL_H__
#define __CALL_H__

#include "RValue.h"
#include <vector>

namespace syntax
{
    class Call : public RValue
    {
    public:
        virtual Type getType()
        {
            return Node::Type::Call;
        }

        void setName(const std::string & name)
        {
            _name = name;
        }

        void addArgument(const std::shared_ptr<RValue> & rvalue)
        {
            _arguments.push_back(rvalue);
        }

        std::string _name;
        std::vector<std::shared_ptr<RValue>> _arguments;
    };
}

#endif // __CALL_H__