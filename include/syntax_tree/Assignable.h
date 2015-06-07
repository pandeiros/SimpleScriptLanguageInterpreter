#ifndef __ASSIGNABLE_H__
#define __ASSIGNABLE_H__

#include "Node.h"

/*
 *
 */
class Assignable : public Node
{
public:
    typedef std::shared_ptr<Assignable> ptr;
};

#endif // __ASSIGNABLE_H__