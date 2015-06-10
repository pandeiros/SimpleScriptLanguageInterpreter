#ifndef __INTER_CALL_INSTR_H__
#define __INTER_CALL_INSTR_H__

#include <vector>
#include <memory>
#include <string>

#include "Instruction.h"
#include "Assignable.h"
#include "Executable.h"

#include "Library.h"

namespace inter
{
    struct Call: public Instruction, public Assignable
    {
        std::string name;
        std::vector<std::shared_ptr<inter::Assignable>> arguments;

        virtual std::shared_ptr<Literal> execute(
            ScopeInstance* scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions
        )
        {
            std::vector<std::shared_ptr<inter::Literal>> concreteArguments;
            for(auto& it: this->arguments)
            {
                concreteArguments.push_back(it->execute(scope, functions));
            }

            if (functions.count(this->name) == 1)
            {
                return functions.at(this->name)->execute(nullptr, functions, concreteArguments);
            }
            else
            {
                return Library::callFunction(this->name, concreteArguments);
            }
        }
    };
}

#endif // __INTER_CALL_INSTR_H__
