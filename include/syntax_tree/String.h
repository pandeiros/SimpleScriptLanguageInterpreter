#ifndef __STRING_H__
#define __STRING_H__

#include "Literal.h"

namespace syntax
{
    class String : public Literal
    {
    public:
        virtual Type getType()
        {
            return Node::Type::String;
        }
    };
}

#endif // __LITERAL_H_