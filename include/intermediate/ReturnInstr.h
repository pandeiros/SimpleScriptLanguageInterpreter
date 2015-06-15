#ifndef __INTER_RETURN_INSTR_H__
#define __INTER_RETURN_INSTR_H__

#include <memory>

#include "Instruction.h"
#include "Assignable.h"
#include "Executable.h"

namespace inter
{
    class ReturnInstr: public Instruction
    {
    public:
        virtual std::shared_ptr<Literal> execute(
            ScopeInstance* scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions
        )
        {
            if (_isEmpty)
                return std::make_shared<inter::Literal>();  // Maybe nullptr?

            return this->_value->execute(scope, functions);
        }

        virtual bool canReturn()
        {
            return true;
        }

        bool _isEmpty = false;
        std::shared_ptr<Assignable> _value;
    };
}

#endif // __INTER_RETURN_INSTR_H__
