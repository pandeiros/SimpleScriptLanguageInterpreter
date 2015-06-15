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

        virtual int compare(std::shared_ptr<LogicalOperand> other)
        {
            return -2;
        }
    };
}

#endif // __INTER_CONDITION_OPERAND_H__
