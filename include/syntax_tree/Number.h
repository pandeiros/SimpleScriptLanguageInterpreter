#ifndef __NUMBER_H__
#define __NUMBER_H__

#include "Literal.h"

namespace syntax
{
    class Number : public Literal
    {
    public:
        virtual Type getType()
        {
            return Node::Type::Number;
        }

        double _value;
    };
}

#endif // __NUMBER_H__