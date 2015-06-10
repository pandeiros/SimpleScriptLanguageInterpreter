#ifndef __INTER_FUNCTION_H__
#define __INTER_FUNCTION_H__

#include <vector>
#include <memory>
#include <string>

#include "Block.h"
#include "Literal.h"

#include "MessageHandler.h"

namespace inter
{
    struct Function: public Block
    {
        typedef std::shared_ptr<Function> ptr;

        std::string name;

        virtual std::shared_ptr<Literal> execute(
            ScopeInstance * scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions
        )
        {
            MessageHandler::error(
                std::string("Cannot execute function without parameters, fatal error")
            );
            return nullptr;
        }

        virtual std::shared_ptr<Literal> execute(
            ScopeInstance * scope,
            std::unordered_map<std::string, std::shared_ptr<Function>>& functions,
            std::vector<std::shared_ptr<inter::Literal>> arguments
        )
        {
            auto thisScope = this->scopePrototype.instantiate(scope);
            unsigned int argIdx = 0;
            for(auto& argument: arguments)
            {
                std::shared_ptr<inter::Literal> copy = std::make_shared<inter::Literal>();
                copy->data = argument->data;

                thisScope.variables.at(thisScope.varOrder.at(argIdx)) = copy;
                argIdx++;
            }

            for(auto& it: this->instructions)
            {
                auto result = it->execute(&thisScope, functions);
                if (result && result->loopJump)
                {
                    MessageHandler::error(
                        std::string("Break/continue outside of loop")
                    );
                }
                if (result && it->canDoReturn())
                {
                    return result;
                }
            }

            MessageHandler::error(
                std::string("No return, fatal error")
            );
            return nullptr;
        };
    };
}

#endif // __INTER_FUNCTION_H__
