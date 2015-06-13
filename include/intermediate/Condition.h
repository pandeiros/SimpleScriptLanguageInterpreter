#ifndef __INTER_CONDITION_H__
#define __INTER_CONDITION_H__

#include <vector>
#include <memory>
#include <iostream>

#include "Executable.h"
#include "ConditionOperand.h"
#include "Literal.h"
#include "TokenType.h"

#include "MessageHandler.h"

namespace inter
{
    class Condition : public ConditionOperand
    {
    public:
        bool negated = false;
        TokenType operation = TokenType::Undefined;
        std::vector<std::shared_ptr<ConditionOperand>> operands;

        virtual std::shared_ptr<Literal> execute(ScopeInstance * scope,
                                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions)
        {
            // TODO change
            return nullptr;
        }
    };
}

/*if (this->operation == TokenType::Undefined)
{
if (!this->negated)
{
return this->operands.at(0)->execute(scope, functions);
}
else
{
auto result = std::make_shared<Literal>();
result->castedToBool = true;
result->data = { { this->operands.at(0)->execute(scope, functions)->isTruthy() ? 0.0 : 1.0 } };
return result;
}
}

else if (this->operation == TokenType::Or)
{
auto result = std::make_shared<inter::Literal>();
result->castedToBool = true;
for(auto& it: this->operands)
{
if (it->execute(scope, functions)->isTruthy())
{
result->data = { { 1.0 } };
return result;
}
}
result->data = { { 0.0 } };
return result;
}
else if (this->operation == TokenType::And)
{
auto result = std::make_shared<inter::Literal>();
result->castedToBool = true;
for(auto& it: this->operands)
{
if (!it->execute(scope, functions)->isTruthy())
{
result->data = { { 0.0 } };
return result;
}
}
result->data = { { 1.0 } };
return result;
}
else if (this->operation == TokenType::Equality)
{
auto result = std::make_shared<inter::Literal>();
result->castedToBool = true;

auto left = this->operands.at(0)->execute(scope, functions);
auto right = this->operands.at(1)->execute(scope, functions);

if (left->castedToBool && right->castedToBool)
{
result->data = { { left->isTruthy() == right->isTruthy() ? 1.0 : 0.0 } };
}
else if (!left->castedToBool && !right->castedToBool)
{
result->data = { { *left == *right ? 1.0 : 0.0 } };
}
else
{
MessageHandler::error(
std::string("Tried to compare matrix with bool matrix")
);
return nullptr;
}

return result;
}
else if (this->operation == TokenType::Inequality)
{
auto result = std::make_shared<inter::Literal>();
result->castedToBool = true;

auto left = this->operands.at(0)->execute(scope, functions);
auto right = this->operands.at(1)->execute(scope, functions);

if (left->castedToBool && right->castedToBool)
{
result->data = { { left->isTruthy() != right->isTruthy() ? 1.0 : 0.0 } };
}
else if (!left->castedToBool && !right->castedToBool)
{
result->data = { { *left != *right ? 1.0 : 0.0 } };
}
else
{
MessageHandler::error(
std::string("Tried to compare matrix with bool matrix")
);
return nullptr;
}

return result;
}
else if (this->operation == TokenType::Less)
{
auto result = std::make_shared<inter::Literal>();
result->castedToBool = true;

auto left = this->operands.at(0)->execute(scope, functions);
auto right = this->operands.at(1)->execute(scope, functions);

if (!left->castedToBool && !right->castedToBool)
{
result->data = { { *left < *right ? 1.0 : 0.0 } };
}
else
{
MessageHandler::error(
std::string("Cannot compare bool matrices")
);
return nullptr;
}

return result;
}
else if (this->operation == TokenType::LessOrEqual)
{
auto result = std::make_shared<inter::Literal>();
result->castedToBool = true;

auto left = this->operands.at(0)->execute(scope, functions);
auto right = this->operands.at(1)->execute(scope, functions);

if (!left->castedToBool && !right->castedToBool)
{
result->data = { { *left <= *right ? 1.0 : 0.0 } };
}
else
{
MessageHandler::error(
std::string("Cannot compare bool matrices")
);
return nullptr;
}

return result;
}
else if (this->operation == TokenType::Greater)
{
auto result = std::make_shared<inter::Literal>();
result->castedToBool = true;

auto left = this->operands.at(0)->execute(scope, functions);
auto right = this->operands.at(1)->execute(scope, functions);

if (!left->castedToBool && !right->castedToBool)
{
result->data = { { *left > *right ? 1.0 : 0.0 } };
}
else
{
MessageHandler::error(
std::string("Cannot compare bool matrices")
);
return nullptr;
}

return result;
}
else if (this->operation == TokenType::GreaterOrEqual)
{
auto result = std::make_shared<inter::Literal>();
result->castedToBool = true;

auto left = this->operands.at(0)->execute(scope, functions);
auto right = this->operands.at(1)->execute(scope, functions);

if (!left->castedToBool && !right->castedToBool)
{
result->data = { { *left >= *right ? 1.0 : 0.0 } };
}
else
{
MessageHandler::error(
std::string("Cannot compare bool matrices")
);
return nullptr;
}

return result;
}
else
{
MessageHandler::error(
std::string("Invalid condition operator")
);
return nullptr;
}*/

#endif // __INTER_CONDITION_H__
