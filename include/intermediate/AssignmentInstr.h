#ifndef __INTER_ASSIGNMENT_INSTR_H__
#define __INTER_ASSIGNMENT_INSTR_H__

#include <memory>

#include "Instruction.h"
#include "intermediate/Variable.h"
#include "Assignable.h"
#include "Executable.h"

namespace inter
{
    struct AssignmentInstr : public inter::Instruction
    {
        virtual std::shared_ptr<Literal> execute(
            ScopeInstance * scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions
            )
        {
            scope->setVariable(_variable->_name, _value->execute(scope, functions));
            _variable->_isDefined = true;
            return nullptr;
        }

        std::shared_ptr<inter::Variable> _variable = std::make_shared<inter::Variable>();
        std::shared_ptr<inter::Assignable> _value;
    };
}

#endif // __INTER_ASSIGNMENT_INSTR_H__
