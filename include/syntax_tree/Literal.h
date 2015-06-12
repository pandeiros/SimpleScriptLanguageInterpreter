#ifndef __LITERAL_H__
#define __LITERAL_H__

#include "Node.h"

namespace syntax
{
    class Literal : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::Literal;
        }

        //double value;
    };
}

#endif // __LITERAL_H__