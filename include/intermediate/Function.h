#ifndef __INTER_FUNCTION_H__
#define __INTER_FUNCTION_H__

#include <vector>
#include <memory>
#include <string>

#include "Block.h"
#include "intermediate/Literal.h"

#include "MessageHandler.h"

namespace inter
{
    class Function : public Block
    {
    public:
        virtual std::shared_ptr<Literal> execute(ScopeInstance * scope,
                                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions)
        {
            MessageHandler::error(std::string("Cannot execute function without parameters, fatal error"));
            return nullptr;
        }

        virtual std::shared_ptr<Literal> execute(ScopeInstance * scope,
                                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions,
                                                 std::vector<std::shared_ptr<inter::Literal>> arguments)
        {
            auto scopeInstance = _scopePrototype.instantiate(scope);
            unsigned int argumentIndex = 0;
            for (auto & argument : arguments)
            {
                std::shared_ptr<inter::Literal> copy = std::make_shared<inter::Literal>();
                copy->_data = argument->_data;

                std::string name = scopeInstance._variableOrder.at(argumentIndex);

                for (auto & var : scopeInstance._variables)
                {
                    if (var.second.first._name == name)
                        var.second.second = copy;
                }
                ++argumentIndex;
            }

            for (auto & instruction : _instructions)
            {
                auto result = instruction->execute(&scopeInstance, functions);

                if (result && instruction->canReturn())
                {
                    return result;
                }
            }

            MessageHandler::error(std::string("Return instruction missing!"));
            return nullptr;
        };

        bool _isMain = false;
        std::string _name;
    };
}

#endif // __INTER_FUNCTION_H__
