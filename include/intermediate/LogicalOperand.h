#ifndef __INTER_CONDITION_OPERAND_H__
#define __INTER_CONDITION_OPERAND_H__

#include "Assignable.h"

namespace inter
{
    struct LogicalOperand: virtual public Assignable
    {
        virtual bool isEqualToTrue()
        {
            return false;
        }
    };
}

#endif // __INTER_CONDITION_OPERAND_H__
