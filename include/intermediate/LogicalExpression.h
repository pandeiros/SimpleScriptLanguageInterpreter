#ifndef __INTER_CONDITION_H__
#define __INTER_CONDITION_H__

#include <vector>
#include <memory>
#include <iostream>

#include "Executable.h"
#include "LogicalOperand.h"
#include "intermediate/Literal.h"
#include "TokenType.h"

#include "MessageHandler.h"

namespace inter
{
    // TODO Change to LogicalExpression
    class LogicalExpression : public LogicalOperand
    {
    public:
        bool negated = false;
        TokenType operation = TokenType::Undefined;
        std::vector<std::shared_ptr<LogicalOperand>> operands;

        virtual std::shared_ptr<Literal> execute(ScopeInstance * scope,
                                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions)
        {
            auto result = std::make_shared<Literal>();
            result->_type = "bool";

            if (this->operation == TokenType::Undefined)
            {
                if (!this->negated)
                {
                    return this->operands.at(0)->execute(scope, functions);
                }
                else
                {
                    result->_boolValue = this->operands.at(0)->execute(scope, functions)->isEqualToTrue() ? false : true;
                    return result;
                }
            }
            else if (this->operation == TokenType::Or)
            {
                for (auto & operand : this->operands)
                {
                    if (operand->execute(scope, functions)->isEqualToTrue())
                    {
                        result->_boolValue = true;
                        return result;
                    }
                }
                return result;
            }
            else if (this->operation == TokenType::And)
            {
                for (auto & operand : this->operands)
                {
                    if (!operand->execute(scope, functions)->isEqualToTrue())
                    {
                        result->_boolValue = false;
                        return result;
                    }
                }
                result->_boolValue = true;
                return result;
            }
            else if (this->operation == TokenType::Equality)
            {
                auto left = this->operands.at(0)->execute(scope, functions);
                auto right = this->operands.at(1)->execute(scope, functions);

                int comparison = left->compare(right);
                if (comparison == 0)
                {
                    result->_boolValue = true;
                }
                else if (comparison != -2)
                    result->_boolValue = false;
                else
                {
                    MessageHandler::error(std::string("Comparison (Equality) failed."));
                    return nullptr;
                }

                return result;
            }
            else if (this->operation == TokenType::Inequality)
            {
                auto left = this->operands.at(0)->execute(scope, functions);
                auto right = this->operands.at(1)->execute(scope, functions);

                int comparison = left->compare(right);
                if (comparison != -2)
                {
                    if (comparison != 0)
                    {
                        result->_boolValue = true;
                    }
                    else
                        result->_boolValue = false;
                }
                else
                {
                    MessageHandler::error(std::string("Comparison (Inequality) failed."));
                    return nullptr;
                }

                return result;
            }
            else if (this->operation == TokenType::Less)
            {
                auto left = this->operands.at(0)->execute(scope, functions);
                auto right = this->operands.at(1)->execute(scope, functions);

                int comparison = left->compare(right);
                if (comparison != -2)
                {
                    if (comparison == -1)
                    {
                        result->_boolValue = true;
                    }
                    else
                        result->_boolValue = false;
                }
                else
                {
                    MessageHandler::error(std::string("Comparison (Less) failed."));
                    return nullptr;
                }

                return result;
            }
            else if (this->operation == TokenType::LessOrEqual)
            {
                auto left = this->operands.at(0)->execute(scope, functions);
                auto right = this->operands.at(1)->execute(scope, functions);

                int comparison = left->compare(right);
                if (comparison != -2)
                {
                    if (comparison <= 0)
                    {
                        result->_boolValue = true;
                    }
                    else
                        result->_boolValue = false;
                }
                else
                {
                    MessageHandler::error(std::string("Comparison (LessOrEqual) failed."));
                    return nullptr;
                }

                return result;
            }
            else if (this->operation == TokenType::Greater)
            {
                auto left = this->operands.at(0)->execute(scope, functions);
                auto right = this->operands.at(1)->execute(scope, functions);

                int comparison = left->compare(right);
                if (comparison != -2)
                {
                    if (comparison == 1)
                    {
                        result->_boolValue = true;
                    }
                    else
                        result->_boolValue = false;
                }
                else
                {
                    MessageHandler::error(std::string("Comparison (Greater) failed."));
                    return nullptr;
                }

                return result;
            }
            else if (this->operation == TokenType::GreaterOrEqual)
            {
                auto left = this->operands.at(0)->execute(scope, functions);
                auto right = this->operands.at(1)->execute(scope, functions);

                int comparison = left->compare(right);
                if (comparison != -2)
                {
                    if (comparison >= 0)
                    {
                        result->_boolValue = true;
                    }
                    else
                        result->_boolValue = false;
                }
                else
                {
                    MessageHandler::error(std::string("Comparison (GreaterOrEqual) failed."));
                    return nullptr;
                }

                return result;
            }
            else
            {
                MessageHandler::error(std::string("Invalid Logical operator!"));
                return nullptr;
            }
        }
    };
}

#endif // __INTER_CONDITION_H__
