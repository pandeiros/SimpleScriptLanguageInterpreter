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
            if (_boolValue || _intValue == 1 || _floatValue == 1.0)
                return true;
            else
                return false;
        }

        std::string _type = "";
        bool _boolValue = false;
        int _intValue = 0;
        double _floatValue = 0.0;
        std::string _stringValue = "";
    };
}

#endif  // __INTER_LITERAL_H__
