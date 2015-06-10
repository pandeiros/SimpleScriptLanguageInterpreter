#ifndef __INTER_SCOPE_INST_H__
#define __INTER_SCOPE_INST_H__

#include <map>
#include <string>
#include <memory>
#include <vector>

#include "MessageHandler.h"

typedef std::pair<std::string, std::string> VarType;

class Var : public std::string
{
public:
    Var() = default;
    Var(std::string type, std::string name) : name(name), type(type)
    {}

    std::string type;
    std::string name;
};

namespace inter
{
    struct Literal;

    struct ScopeInstance
    {
        ScopeInstance* upperScope = nullptr;
        std::map<Var, std::shared_ptr<Literal>> variables;
        std::vector<std::string> varOrder;

        std::shared_ptr<Literal> getVariable(const std::string & name)
        {
            for (auto & var : variables)
            {
                if (var.first.name == name)
                    return var.second;
            }

            if (this->upperScope != nullptr)
            {
                return this->upperScope->getVariable(name);
            }

            return nullptr;
        }

        void setVariable(const std::string & name, std::shared_ptr<Literal> literal)
        {
            for (auto & var : variables)
            {
                if (var.first.name == name)
                {
                    var.second = literal;
                    return;
                }
            }

            if (this->upperScope != nullptr)
            {
                this->upperScope->setVariable(name, literal);
                return;
            }

            MessageHandler::error(std::string("Setting undefined variable"));
        }
    };
}

#endif  // __INTER_SCOPE_INST_H__
