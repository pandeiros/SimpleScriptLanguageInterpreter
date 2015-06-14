#ifndef __INTER_SCOPE_PROTO_H__
#define __INTER_SCOPE_PROTO_H__

#include <unordered_map>
#include <map>
#include <string>
#include <memory>
#include <vector>

//#include "intermediate/Variable.h"
#include "ScopeInstance.h"

namespace inter
{
    class ScopePrototype
    {
    public:
        bool addVariable(const std::string & type, const std::string & name, bool isConstant = false)
        {
            if (this->hasVariable(name))
            {
                return false;
            }

            std::pair<Variable, bool> newPair(Variable(type, name, true), false);
            _variables.insert(newPair);
            _variableOrder.push_back(name);
            return true;
        }

        bool * getVariable(const std::string & name)
        {
            for (auto & var : _variables)
            {
                if (var.first._name == name)
                    return &(var.second);
            }

            if (_upperScope != nullptr)
            {
                return _upperScope->getVariable(name);
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

            for (auto & var : _variables)
            {
                if (var.first._name == name)
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

            for (auto & var : _variables)
            {
                if (var.first._name == name)
                    return var.second;
            }

            return false;
        }

        bool isConstant(const std::string & name)
        {
            for (auto & var : _variables)
            {
                if (var.first._name == name)
                    return var.first._isConstant;
            }

            return false;
        }

        ScopeInstance instantiate(ScopeInstance * upperScope)
        {
            auto instance = ScopeInstance();
            instance._upperScope = upperScope;
            instance._variableOrder = _variableOrder;

            for(auto & it : _variables)
            {
                std::pair<Variable, std::shared_ptr<Literal>> newPair(it.first, std::make_shared<inter::Literal>());
                instance._variables.insert(newPair);
            }

            return instance;
        }

        ScopePrototype * _upperScope = nullptr;
        std::map<Variable, bool> _variables;
        std::vector<std::string> _variableOrder;

    };
}

#endif // __INTER_SCOPE_PROTO_H__
