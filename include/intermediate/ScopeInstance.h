#ifndef __INTER_SCOPE_INST_H__
#define __INTER_SCOPE_INST_H__

#include <map>
#include <string>
#include <memory>
#include <vector>

#include "intermediate/Variable.h"
//#include "intermediate/Literal.h"
//#include "Executable.h"

#include "MessageHandler.h"
//
//class Var : public std::string
//{
//public:
//    Var() = default;
//    Var(std::string type, std::string name) : _name(name), _type(type)
//    {}
//
//    std::string _type;
//    std::string _name;
//};

namespace inter
{
    //class Variable;

    class ScopeInstance
    {
    public:
        std::shared_ptr<Literal> getVariable(const std::string & name)
        {
            for (auto & var : _variables)
            {
                if (var.first == name)
                    return var.second.second;
            }

            if (_upperScope != nullptr)
            {
                return _upperScope->getVariable(name);
            }

            MessageHandler::error(std::string("Accessing undefined variable: ").append(name));
            return nullptr;
        }

        void setVariable(const std::string & name, std::shared_ptr<Literal> literal)
        {
            for (auto & var : _variables)
            {
                if (var.first == name)
                {
                    var.second.second = literal;
                    return;
                }
            }

            if (_upperScope != nullptr)
            {
                _upperScope->setVariable(name, literal);
                return;
            }

            MessageHandler::error(std::string("Modifying undefined variable!"));
        }

        ScopeInstance * _upperScope = nullptr;
        std::map<std::string, std::pair<Variable, std::shared_ptr<Literal> > > _variables;
        std::vector<std::string> _variableOrder;
    };
}

#endif  // __INTER_SCOPE_INST_H__
