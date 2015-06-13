#ifndef __LITERAL_H__
#define __LITERAL_H__

#include "RValue.h"

namespace syntax
{
    class Literal : public RValue
    {
    public:
        virtual Type getType()
        {
            return Node::Type::Literal;
        }
    };
}

#endif // __LITERAL_H__