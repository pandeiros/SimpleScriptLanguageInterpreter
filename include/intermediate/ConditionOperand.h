#ifndef __INTER_CONDITION_OPERAND_H__
#define __INTER_CONDITION_OPERAND_H__

#include "Executable.h"

namespace inter
{
    struct ConditionOperand: virtual public Executable
    {
        virtual bool isTruthy()
        {
            return false;
        }
    };
}

#endif // __INTER_CONDITION_OPERAND_H__
