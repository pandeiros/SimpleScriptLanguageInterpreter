#ifndef __INTER_ASSIGNABLE_H__
#define __INTER_ASSIGNABLE_H__

#include "Executable.h"

namespace inter
{
    class Assignable: public Executable
    {
    public:
        virtual bool isEqualToTrue()
        {
            return false;
        }
    };
}

#endif  // __INTER_ASSIGNABLE_H__
