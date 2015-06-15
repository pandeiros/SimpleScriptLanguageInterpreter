#ifndef __INTER_VARIABLE_H__
#define __INTER_VARIABLE_H__

#include <vector>
#include <memory>
#include "LogicalOperand.h"
#include "ArithmeticOperand.h"

namespace inter
{
    class ScopeInstance;
    class Literal;

    class Variable : public LogicalOperand, public ArithmeticOperand
    {
    public:
        Variable() = default;
        Variable(std::string type, std::string name, bool isConstant = false)
            : _name(name), _type(type), _isConstant(isConstant)
        {}

        virtual std::shared_ptr<Literal> execute(ScopeInstance * scope,
                                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions);


        std::string _type;
        std::string _name;
        bool _isDefined = false;
        bool _isConstant = false;
    };
};

#endif // __INTER_VARIABLE_H__
