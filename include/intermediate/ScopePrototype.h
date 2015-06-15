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
    class ScopePrototype
    {
    public:
        bool addVariable(const std::string & type, const std::string & name, bool isConstant = false)
        {
            if (this->hasVariable(name))
            {
                return false;
            }

            //std::pair<std::string, Variable> newPair(name, Variable(type, name, isConstant));
            //Variable var(type, name, isConstant);
            auto & result = _variables.emplace(name, Variable(type, name, isConstant));
            if (result.second)
                _variableOrder.push_back(name);
            else
                MessageHandler::error("Variable insertion into scope failed!");
            return true;
        }

        bool * getVariable(const std::string & name)
        {
            for (auto & var : _variables)
            {
                if (var.second._name == name)
                    return &(var.second._isDefined);
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
                if (var.second._name == name)
                {
                    var.second._isDefined = true;
                    break;
                }
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
                if (var.second._name == name)
                    return var.second._isDefined;
            }

            if (_upperScope != nullptr)
            {
                return _upperScope->isVariableDefined(name);
            }

            return false;
        }

        bool isConstant(const std::string & name)
        {
            for (auto & var : _variables)
            {
                if (var.second._name == name)
                    return var.second._isConstant;
            }

            return false;
        }

        std::string getType(const std::string & name)
        {
            for (auto & var : _variables)
            {
                if (var.second._name == name)
                    return var.second._type;
            }

            return "";
        }

        ScopeInstance instantiate(ScopeInstance * upperScope)
        {
            auto instance = ScopeInstance();
            instance._upperScope = upperScope;
            instance._variableOrder = _variableOrder;

            for (auto & variable : _variables)
            {
                std::pair<std::string, std::pair<std::shared_ptr<Variable>, std::shared_ptr<Literal>>> newPair(
                    variable.first,
                    std::make_pair(std::make_shared<inter::Variable>(variable.second), std::make_shared<inter::Literal>()));

                instance._variables.insert(newPair);
            }

            return instance;
        }

        ScopePrototype * _upperScope = nullptr;
        std::unordered_map<std::string, Variable> _variables;
        std::vector<std::string> _variableOrder;

    };
}

#endif // __INTER_SCOPE_PROTO_H__
