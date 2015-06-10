#ifndef __INTER_SCOPE_INST_H__
#define __INTER_SCOPE_INST_H__

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#include "MessageHandler.h"

namespace inter
{
    struct Literal;

    struct ScopeInstance
    {
        ScopeInstance* upperScope = nullptr;
        std::unordered_map<std::string, std::shared_ptr<Literal>> variables;
        std::vector<std::string> varOrder;

        std::shared_ptr<Literal> getVariable(const std::string& name)
        {
            auto it = this->variables.find(name);
            if (it != this->variables.end())
            {
                return it->second;
            }

            if (this->upperScope != nullptr)
            {
                return this->upperScope->getVariable(name);
            }

            return nullptr;
        }

        void setVariable(const std::string& name, std::shared_ptr<Literal> literal)
        {
            auto it = this->variables.find(name);
            if (it != this->variables.end())
            {
                it->second = literal;
                return;
            }

            if (this->upperScope != nullptr)
            {
                this->upperScope->setVariable(name, literal);
                return;
            }

            MessageHandler::error(
                std::string("Setting undefined variable")
            );
            return ;
        }
    };
}

#endif  // __INTER_SCOPE_INST_H__
