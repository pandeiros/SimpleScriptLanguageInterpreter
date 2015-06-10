#ifndef __LITERAL_H__
#define __LITERAL_H__

#include "Node.h"

namespace syntax
{
    class Literal : public Node
    {
    public:
        void pushValue(const double & value)
        {
            this->value = value;
        }
        virtual Type getType()
        {
            return Node::Type::Number;
        }

        double value;
    };
}

#endif // __LITERAL_H__