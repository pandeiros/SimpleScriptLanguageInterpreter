#ifndef __STRING_H__
#define __STRING_H__

#include "Literal.h"
#include <string>

namespace syntax
{
    class String : public Literal
    {
    public:
        virtual Type getType()
        {
            return Node::Type::String;
        }

        std::string _value = "";
    };
}

#endif // __STRING_H__