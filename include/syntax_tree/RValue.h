#ifndef __R_VALUE_H__
#define __R_VALUE_H__

#include "Node.h"

namespace syntax
{
    class RValue : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::Assignable;
        }
    };
}

#endif // __R_VALUE_H__