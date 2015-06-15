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
            if (!_variable->_isDefined)
            {
                scope->setVariable(_variable->_name, _value->execute(scope, functions));
                return nullptr;
            }

            std::shared_ptr <Literal> lit = scope->getVariable(_variable->_name);
            auto & result = _value->execute(scope, functions);

            return nullptr;
        }

        /*if (!this->_variable->)
        {
        scope->setVariable(variable->name, value->execute(scope, functions));
        return nullptr;
        }

        std::shared_ptr<Literal> ref = scope->getVariable(variable->name);
        auto indexArgVal = this->variable->indexArg1->execute(scope, functions);
        if (indexArgVal->getSize() != std::make_pair<unsigned int, unsigned int>(1, 1))
        {
        MessageHandler::error(
        std::string("Cannot call index access using matrix")
        );
        return nullptr;
        }
        if (indexArgVal->data.at(0).at(0) < 0)
        {
        MessageHandler::error(
        std::string("Negative index (y)")
        );
        return nullptr;
        }
        if (ref->data.size() <= indexArgVal->data.at(0).at(0))
        {
        MessageHandler::error(
        std::string("Out of bound access (y)")
        );
        return nullptr;
        }

        auto& rowRef = ref->data.at(indexArgVal->data.at(0).at(0));
        if (!this->variable->indexArg2)
        {
        auto newVal = value->execute(scope, functions);

        if (newVal->getSize() != std::make_pair<unsigned int, unsigned int>(1, rowRef.size()))
        {
        MessageHandler::error(
        std::string("Invalid assignable size (y)")
        );
        return nullptr;
        }

        rowRef = newVal->data.at(0);
        return nullptr;
        }

        indexArgVal = this->variable->indexArg2->execute(scope, functions);
        if (indexArgVal->getSize() != std::make_pair<unsigned int, unsigned int>(1, 1))
        {
        MessageHandler::error(
        std::string("Cannot call index access using matrix")
        );
        return nullptr;
        }
        if (indexArgVal->data.at(0).at(0) < 0)
        {
        MessageHandler::error(
        std::string("Negative index (x)")
        );
        return nullptr;
        }
        if (rowRef.size() <= indexArgVal->data.at(0).at(0))
        {
        MessageHandler::error(
        std::string("Out of bound access (x)")
        );
        return nullptr;
        }

        auto newVal = value->execute(scope, functions);

        if (newVal->getSize() != std::make_pair<unsigned int, unsigned int>(1, 1))
        {
        MessageHandler::error(
        std::string("Invalid assignable size (x)")
        );
        return nullptr;
        }

        rowRef.at(indexArgVal->data.at(0).at(0)) = newVal->data.at(0).at(0);

        return nullptr;*/


        std::shared_ptr<inter::Variable> _variable = std::make_shared<inter::Variable>();
        std::shared_ptr<inter::Assignable> _value;
    };
}

#endif // __INTER_ASSIGNMENT_INSTR_H__
