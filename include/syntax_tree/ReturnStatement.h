#ifndef __RETURN_STATEMENT_H__
#define __RETURN_STATEMENT_H__

#include "Node.h"

namespace syntax
{
    class ReturnStatement : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::ReturnStatement;
        }

        void setValue(const std::shared_ptr<RValue> & rvalue)
        {
            _value = rvalue;
        }

        bool _isEmpty = false;
        std::shared_ptr<RValue> _value;
    };
}

#endif // __RETURN_STATEMENT_H__