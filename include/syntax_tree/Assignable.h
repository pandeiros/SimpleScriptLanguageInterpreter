#ifndef __ASSIGNABLE_H__
#define __ASSIGNABLE_H__

#include "Node.h"

/*
 *
 */
namespace syntax
{
    class Assignable : public Node  // TODO Change to RValue
    {
    public:
        typedef std::shared_ptr<Assignable> ptr;
    };
}

#endif // __ASSIGNABLE_H__