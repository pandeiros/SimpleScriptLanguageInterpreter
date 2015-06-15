#ifndef __INTER_LITERAL_H__
#define __INTER_LITERAL_H__

#include <vector>
#include <memory>
#include <iostream>
#include <utility>

#include "LogicalOperand.h"
#include "ArithmeticOperand.h"

#include "MessageHandler.h"

namespace inter
{
    class Literal : public LogicalOperand, public ArithmeticOperand
    {
    public:
        Literal() = default;

        virtual std::shared_ptr<Literal> execute(ScopeInstance * scope,
                                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions)
        {
            return makeCopy();
        }

        std::shared_ptr<Literal> makeCopy()
        {
            std::shared_ptr<Literal> copy = std::make_shared<Literal>();
            copy->_type = this->_type;
            copy->_boolValue = this->_boolValue;
            copy->_intValue = this->_intValue;
            copy->_floatValue = this->_floatValue;
            copy->_stringValue = this->_stringValue;

            return copy;
        }

        virtual bool isEqualToTrue()
        {
            if (_boolValue || _intValue >= 1 || _floatValue != 0.0)
                return true;
            else
                return false;
        }

        virtual int compare(std::shared_ptr<LogicalOperand> other)
        {
            std::shared_ptr<Literal> lit = std::dynamic_pointer_cast<Literal>(other);

            if (_type == "string" && lit->_type == "string")
                return _stringValue.compare(lit->_stringValue);

            if (_type == "bool" && lit->_type == "bool")
            {
                if (_boolValue == lit->_boolValue)
                    return 0;
                else if (_boolValue == true)
                    return 1;
                else
                    return -1;
            }

            if (_type == "string" && lit->_type != "string" ||
                _type != "string" && lit->_type == "string")
                return -2;

            double val1;
            double val2;

            if (_type == "bool")
                val1 = _boolValue;
            else if (_type == "int")
                val1 = _intValue;
            else
                val1 = _floatValue;

            if (lit->_type == "bool")
                val2 = lit->_boolValue;
            if (lit->_type == "int")
                val2 = lit->_intValue;
            else
                val2 = lit->_floatValue;

            if (val1 < val2)
                return -1;
            else if (val1 > val2)
                return 1;
            else
                return 0;
        }

        std::string _type = "";
        bool _boolValue = false;
        int _intValue = 0;
        double _floatValue = 0.0;
        std::string _stringValue = "";
    };
}

#endif  // __INTER_LITERAL_H__
