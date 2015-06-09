#ifndef __INTER_SCOPE_PROTO_H__
#define __INTER_SCOPE_PROTO_H__

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#include "ScopeInst.h"

namespace inter
{
    struct ScopeProto
    {
        ScopeProto* upperScope = nullptr;
        std::unordered_map<std::string, bool> variables;
        std::vector<std::string> varOrder;

        bool addVariable(const std::string& name)
        {
            if (this->hasVariable(name))
            {
                return false;
            }
            std::pair<std::string, bool> newPair(name, false);
            this->variables.insert(newPair);
            this->varOrder.push_back(name);
            return true;
        }

        bool* getVariable(const std::string& name)
        {
            auto it = this->variables.find(name);
            if (it != this->variables.end())
            {
                return &(it->second);
            }

            if (this->upperScope != nullptr)
            {
                return this->upperScope->getVariable(name);
            }

            return nullptr;
        }

        void setVariableDefined(const std::string& name)
        {
            auto variable = this->getVariable(name);

            if (variable == nullptr)
            {
                return;
            }

            (*variable) = true;
        }

        bool hasVariable(const std::string& name)
        {
            return this->getVariable(name) != nullptr;
        }

        bool isVariableDefined(const std::string& name)
        {
            auto variable = this->getVariable(name);

            if (variable == nullptr)
            {
                return false;
            }

            return (*variable);
        }

        ScopeInst instantiate(ScopeInst* upperScope)
        {
            auto instance = ScopeInst();
            instance.upperScope = upperScope;
            instance.varOrder = this->varOrder;

            for(auto& it: this->variables)
            {
                std::pair<std::string, std::shared_ptr<Literal>> newPair(it.first, std::make_shared<inter::Literal>());
                instance.variables.insert(newPair);
            }

            return instance;
        }
    };
}

#endif // __INTER_SCOPE_PROTO_H__
