#ifndef __INTER_LITERAL_H__
#define __INTER_LITERAL_H__

#include <vector>
#include <memory>
#include <iostream>
#include <utility>

#include "ConditionOperand.h"
#include "ExpressionOperand.h"
#include "Executable.h"
//#include "intermediate/Function.h"

#include "MessageHandler.h"

namespace inter
{
    class Literal : public ConditionOperand, public ExpressionOperand
    {
    public:
        virtual std::shared_ptr<Literal> execute(ScopeInstance * scope,
                                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions)
        {
            /* std::shared_ptr<Literal> copy = std::make_shared<Literal>();
             copy->data = this->data;

             return copy;*/

            // TODO CHange
            return nullptr;
        }

        std::vector<std::vector<double>> _data = {};
        bool _castedToBool = false;
    };
}


/*const std::pair<unsigned int, unsigned int> getSize() const
{
return std::make_pair<unsigned int, unsigned int>(this->data.size(), this->data.at(0).size());
}

virtual bool isTruthy()
{
for(auto& it: this->data)
{
for(auto& rowIt: it)
{
if (rowIt != 0)
{
return true;
}
}
}

return false;
}

bool operator==(const Literal& rhs)
{
this->protectSizes(rhs);

for(auto y = 0; y < this->data.size(); ++y)
{
for(auto x = 0; x < this->data.at(0).size(); ++x)
{
if (this->data.at(y).at(x) != rhs.data.at(y).at(x))
{
return false;
}
}
}

return true;
}

bool operator!=(const Literal& rhs)
{
return !((*this) == rhs);
}

bool operator<(const Literal& rhs)
{
this->protectRelationOp(rhs);

return this->data.at(0).at(0) < rhs.data.at(0).at(0);
}
bool operator<=(const Literal& rhs)
{
this->protectRelationOp(rhs);

return this->data.at(0).at(0) <= rhs.data.at(0).at(0);
}
bool operator>(const Literal& rhs)
{
this->protectRelationOp(rhs);

return this->data.at(0).at(0) > rhs.data.at(0).at(0);
}
bool operator>=(const Literal& rhs)
{
this->protectRelationOp(rhs);

return this->data.at(0).at(0) >= rhs.data.at(0).at(0);
}

Literal& operator+=(const Literal& rhs)
{
this->protectSizes(rhs);

for(auto y = 0; y < this->data.size(); ++y)
{
for(auto x = 0; x < this->data.at(0).size(); ++x)
{
this->data.at(y).at(x) += rhs.data.at(y).at(x);
}
}

return *this;
}
Literal& operator-=(const Literal& rhs)
{
this->protectSizes(rhs);

for(auto y = 0; y < this->data.size(); ++y)
{
for(auto x = 0; x < this->data.at(0).size(); ++x)
{
this->data.at(y).at(x) -= rhs.data.at(y).at(x);
}
}

return *this;
}
Literal& operator*=(const Literal& rhs)
{
this->protectMultiplicativeOp(rhs);

this->data.at(0).at(0) *= rhs.data.at(0).at(0);

return *this;
}
Literal& operator/=(const Literal& rhs)
{
this->protectMultiplicativeOp(rhs);

this->data.at(0).at(0) /= rhs.data.at(0).at(0);

return *this;
}
Literal& operator%=(const Literal& rhs)
{
this->protectMultiplicativeOp(rhs);

if (rhs.data.at(0).at(0) == 0)
{
MessageHandler::error(
std::string("Modulus of division by zero is invalid")
);
return *this;
}

this->data.at(0).at(0) = ((int) this->data.at(0).at(0)) % ((int) rhs.data.at(0).at(0));

return *this;
}

private:
void protectSizes(const Literal& rhs)
{
if (this->getSize() != rhs.getSize())
{
MessageHandler::error(
std::string("Used matrices have different sizes")
);
return;
}
}
void protectRelationOp(const Literal& rhs)
{
if (this->getSize() != std::make_pair<unsigned int, unsigned int>(1, 1))
{
MessageHandler::error(
std::string("Comparison operator makes no sense for matrices")
);
return;
}
if (this->getSize() != rhs.getSize())
{
MessageHandler::error(
std::string("Comparison operator makes no sense for matrices")
);
return;
}
}
void protectMultiplicativeOp(const Literal& rhs)
{
if (this->getSize() != std::make_pair<unsigned int, unsigned int>(1, 1))
{
MessageHandler::error(
std::string("Multiplicative operator makes no sense for matrices")
);
return;
}
if (this->getSize() != rhs.getSize())
{
MessageHandler::error(
std::string("Multiplicative operator makes no sense for matrices")
);
return;
}
}*/

#endif  // __INTER_LITERAL_H__
