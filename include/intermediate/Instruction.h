#ifndef __INTER_INSTRUCTION_H__
#define __INTER_INSTRUCTION_H__

#include "Executable.h"

namespace inter
{
    struct Instruction: virtual public Executable
    {
        virtual bool canReturn()
        {
            return false;
        }
    };
}

#endif // __INTER_INSTRUCTION_H__
