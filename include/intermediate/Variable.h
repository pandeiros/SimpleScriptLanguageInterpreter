#ifndef __INTER_VARIABLE_H__
#define __INTER_VARIABLE_H__

#include <vector>
#include <memory>

#include "intermediate/Function.h"
#include "LogicalOperand.h"
#include "ArithmeticOperand.h"

namespace inter
{
    class Variable : public LogicalOperand, public ArithmeticOperand
    {
    public:
        Variable() = default;
        Variable(std::string type, std::string name, bool isConstant = false) 
            : _name(name), _type(type), _isConstant(isConstant)
        {}

        virtual std::shared_ptr<Literal> execute(ScopeInstance * scope,
                                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions)
        {
            // TODO Change
            return nullptr;
        }

        std::string _type;
        std::string _name;
        bool _isDefined = false;
        bool _isConstant = false;
    };
};

#endif // __INTER_VARIABLE_H__

/* if (!this->indexArg1)
 {
 std::shared_ptr<Literal> ref = scope->getVariable(name);
 std::shared_ptr<Literal> copy = std::make_shared<Literal>();
 copy->data = ref->data;
 return copy;
 }

 std::shared_ptr<Literal> ref = scope->getVariable(name);
 std::shared_ptr<Literal> copy = std::make_shared<Literal>();
 copy->data = ref->data;
 auto indexArgVal = this->indexArg1->execute(scope, functions);
 if (indexArgVal->getSize() != std::make_pair<unsigned int, unsigned int>(1, 1))
 {
 MessageHandler::error(
 std::string("Cannot call index access using matrix")
 );
 return nullptr;
 }
 if (indexArgVal->data.at(0).at(0) < 0)
 {
 MessageHandler::error(
 std::string("Negative index (y)")
 );
 return nullptr;
 }
 if (copy->data.size() <= indexArgVal->data.at(0).at(0))
 {
 MessageHandler::error(
 std::string("Out of bound access (y)")
 );
 return nullptr;
 }
 copy->data = { copy->data.at(indexArgVal->data.at(0).at(0)) };

 if (!this->indexArg2)
 {
 return copy;
 }

 indexArgVal = this->indexArg2->execute(scope, functions);
 if (indexArgVal->getSize() != std::make_pair<unsigned int, unsigned int>(1, 1))
 {
 MessageHandler::error(
 std::string("Cannot call index access using matrix")
 );
 return nullptr;
 }
 if (indexArgVal->data.at(0).at(0) < 0)
 {
 MessageHandler::error(
 std::string("Negative index (x)")
 );
 return nullptr;
 }
 if (copy->data.at(0).size() <= indexArgVal->data.at(0).at(0))
 {
 MessageHandler::error(
 std::string("Out of bound access (x)")
 );
 return nullptr;
 }
 copy->data = { { copy->data.at(0).at(indexArgVal->data.at(0).at(0)) } };

 return copy; */


