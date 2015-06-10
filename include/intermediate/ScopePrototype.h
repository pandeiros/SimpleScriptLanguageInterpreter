#ifndef __INTER_SCOPE_PROTO_H__
#define __INTER_SCOPE_PROTO_H__

#include <unordered_map>
#include <map>
#include <string>
#include <memory>
#include <vector>

#include "ScopeInstance.h"

namespace inter
{
    struct ScopePrototype
    {
        ScopePrototype * upperScope = nullptr;
        std::map<Var, bool> variables;
        std::vector<std::string> varOrder;

        bool addVariable(const std::string & type, const std::string & name)
        {
            if (this->hasVariable(name))
            {
                return false;
            }
            std::pair<Var, bool> newPair(Var(type, name), false);
            this->variables.insert(newPair);
            this->varOrder.push_back(name);
            return true;
        }

        bool * getVariable(const std::string & name)
        {
            for (auto & var : variables)
            {
                if (var.first.name == name)
                    return &(var.second);
            }

            if (this->upperScope != nullptr)
            {
                return this->upperScope->getVariable(name);
            }

            return nullptr;
        }

        void setVariableDefined(const std::string & name)
        {
            auto variable = this->getVariable(name);

            if (variable == nullptr)
            {
                return;
            }

            for (auto & var : variables)
            {
                if (var.first.name == name)
                    var.second = true;
            }
        }

        bool hasVariable(const std::string & name)
        {
            return this->getVariable(name) != nullptr;
        }

        bool isVariableDefined(const std::string & name)
        {
            auto variable = this->getVariable(name);

            if (variable == nullptr)
            {
                return false;
            }

            for (auto & var : variables)
            {
                if (var.first.name == name)
                    var.second = true;
            }
        }

        ScopeInstance instantiate(ScopeInstance * upperScope)
        {
            auto instance = ScopeInstance();
            instance.upperScope = upperScope;
            instance.varOrder = this->varOrder;

            for(auto & it : this->variables)
            {
                std::pair<Var, std::shared_ptr<Literal>> newPair(it.first, std::make_shared<inter::Literal>());
                instance.variables.insert(newPair);
            }

            return instance;
        }
    };
}

#endif // __INTER_SCOPE_PROTO_H__
