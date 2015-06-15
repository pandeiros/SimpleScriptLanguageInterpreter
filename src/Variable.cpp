#include "intermediate/Variable.h"

#include "intermediate/ScopeInstance.h"
#include "intermediate/Literal.h"

using namespace inter;

std::shared_ptr<Literal> Variable::execute(ScopeInstance * scope,
                                 std::unordered_map<std::string, std::shared_ptr<Function>> & functions)

{
     std::shared_ptr<Literal> value = scope->getVariable(_name);
     if (value != nullptr)
         return value->makeCopy();
     else
         return nullptr;
}