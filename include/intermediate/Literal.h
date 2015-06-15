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
        enum Operation
        {
            ADD,
            SUB,
            MUL,
            DIV,
            MOD
        };

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

        void doOperation(const Literal & other, Operation type)
        {
            if (this->_type == "string" || other._type == "string")
            {
                this->_type = "float";
                this->_floatValue = 0.0;
                MessageHandler::error("Type <string> is not compatible for arithmetical operations.");
                return;
            }

            double val1;
            double val2;
            int conversion1 = 0;
            int conversion2 = 0;
            int max = 0;

            if (_type == "bool")
            {
                conversion1 = 1;
                val1 = _boolValue;
            }
            else if (_type == "int")
            {
                conversion1 = 2;
                val1 = _intValue;
            }
            else
            {
                conversion1 = 3;
                val1 = _floatValue;
            }

            if (other._type == "bool")
            {
                conversion2 = 1;
                val2 = other._boolValue;
            }
            if (other._type == "int")
            {
                conversion2 = 2;
                val2 = other._intValue;
            }
            else
            {
                conversion2 = 3;
                val2 = other._floatValue;
            }

            conversion1 >= conversion2 ? max = conversion1 : max = conversion2;

            if (max == 1)
            {
                switch (type)
                {
                    case ADD:
                        _boolValue = other._boolValue || _boolValue;
                        break;
                    case SUB:
                        _boolValue = !other._boolValue && _boolValue;
                        break;
                    case MUL:
                        _boolValue = other._boolValue && _boolValue;
                        break;
                    case DIV:
                        if (!other._boolValue)
                            MessageHandler::error("Division by zero!");
                        else
                            //_boolValue = _boolValue;  DO NOTHING
                        break;
                    case MOD:
                        if (!_boolValue)
                            MessageHandler::error("Modulo by zero!");
                        else
                            _boolValue = false;
                        break;
                    default:
                        break;
                }
            }
            else if (max == 2)
            {
                switch (type)
                {
                    case ADD:
                        _type = "int";
                        _intValue = val1 + val2;
                        break;
                    case SUB:
                        _type = "int";
                        _intValue = val1 - val2;
                        break;
                    case MUL:
                        _type = "int";
                        _intValue = val1 * val2;
                        break;
                    case DIV:
                        if (!val2)
                            MessageHandler::error("Division by zero!");
                        else
                        {
                            _type = "int";
                            _intValue = val1 / val2;
                        }
                        break;
                    case MOD:
                        if (!val2)
                            MessageHandler::error("Modulo by zero!");
                        else
                        {
                            _type = "int";
                            _intValue = static_cast<int>(val1) % static_cast<int>(val2);
                        }
                        break;
                    default:
                        break;
                }
            }
            else
            {
                switch (type)
                {
                    case ADD:
                        _type = "float";
                        _floatValue = val1 + val2;
                        break;
                    case SUB:
                        _type = "float";
                        _floatValue = val1 - val2;
                        break;
                    case MUL:
                        _type = "float";
                        _floatValue = val1 * val2;
                        break;
                    case DIV:
                        if (!val2)
                            MessageHandler::error("Division by zero!");
                        else
                        {
                            _type = "float";
                            _floatValue = val1 / val2;
                        }
                        break;
                    case MOD:
                        if (other._type == "float")
                            MessageHandler::error("Modulo by floating point number!");
                        else if (_type == "float")
                            MessageHandler::error("Left operand of modulo cannot be floating point number!");
                        break;
                    default:
                        break;
                }
            }
        }

        Literal & operator+=(const Literal & other)
        {
            this->doOperation(other, ADD);
            return *this;
        }

        Literal & operator-=(const Literal& other)
        {
            this->doOperation(other, SUB);
            return *this;
        }

        Literal & operator*=(const Literal& other)
        {
            this->doOperation(other, MUL);
            return *this;
        }

        Literal & operator/=(const Literal& other)
        {
            this->doOperation(other, DIV);
            return *this;
        }

        Literal & operator%=(const Literal& other)
        {
            this->doOperation(other, MOD);
            return *this;
        }

        std::string _type = "";
        bool _boolValue = false;
        int _intValue = 0;
        double _floatValue = 0.0;
        std::string _stringValue = "";
    };
}

#endif  // __INTER_LITERAL_H__
