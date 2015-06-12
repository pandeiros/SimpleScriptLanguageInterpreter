#ifndef __BOOL_H__
#define __BOOL_H__

#include "Literal.h"

namespace syntax
{
    class Bool : public Literal
    {
    public:
        virtual Type getType()
        {
            return Node::Type::Bool;
        }

        bool _value;
    };
}

#endif // __BOOL_H__