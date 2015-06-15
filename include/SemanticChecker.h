#ifndef __SEMANTIC_CHECKER_H__
#define __SEMANTIC_CHECKER_H__

#include "Parser.h"
#include "intermediate/Intermediate.h"

/*
 * Checks semantic of given program syntax tree.
 */
class SemanticChecker
{
public:
    // Initiate function checking
    std::vector<std::shared_ptr<inter::Function>> checkAll(const std::shared_ptr<syntax::Program> & program);

    // Success flag getter.
    inline bool getCheckSucceeded()
    {
        return _processSucceeded;
    }

private:
    // Preliminary checks.
    bool scanFunctionDefinitions();
    bool checkForProgram();

    // Main check function. Traverse all nodes.
    std::vector<std::shared_ptr<inter::Function>> traverseAll();

    // Called on type mismatch.
    void typeFail(const std::string & type, syntax::Node::Type nodeType);

    /* Other check methods. */

    // Functions and blocks/
    std::shared_ptr<inter::Function> checkFunction(syntax::FunctionDefinition & functionDef);
    std::shared_ptr<inter::Block> checkBlock(inter::ScopePrototype & scopePrototype, syntax::StatementBlock & syntaxBlock);

    // Basic instructions.
    void checkVarDeclaration(inter::ScopePrototype & scopePrototype, const std::string & type, const std::string & name);
    void checkConstDeclaration(inter::ScopePrototype & scopePrototype, const std::string & type, const std::string & name);
    std::shared_ptr<inter::AssignmentInstr> checkAssignment(inter::ScopePrototype & scopePrototype, const std::string & variable, syntax::RValue & rvalue);
    std::shared_ptr<inter::AssignmentInstr> checkAssignment(inter::ScopePrototype & scopePrototype, std::shared_ptr<syntax::Variable> & variable, syntax::RValue & rvalue);
    std::shared_ptr<inter::Assignable> checkAssignable(inter::ScopePrototype & scopePrototype, syntax::RValue & rvalue, const std::string & type = "");
    std::shared_ptr<inter::CallInstr> checkFunctionCall(inter::ScopePrototype & scopePrototype, syntax::Call & function);
    std::shared_ptr<inter::ReturnInstr> checkReturnStatement(inter::ScopePrototype & scopePrototype, syntax::RValue & rvalue);

    // Expressions.
    std::shared_ptr<inter::ArithmeticExpression> checkArithmeticExpression(inter::ScopePrototype & scopePrototype, syntax::ArithmeticExpression & expression);
    std::shared_ptr<inter::LogicalExpression> checkLogicalExpression(inter::ScopePrototype & scopePrototype, syntax::LogicalExpression & expression);

    // Variables and values.
    std::shared_ptr<inter::Variable> checkVariable(inter::ScopePrototype & scopePrototype, syntax::Variable & variable);
    std::shared_ptr<inter::Literal> checkLiteral(inter::ScopePrototype & scopePrototype, syntax::Literal & lit, const std::string & type = "");

    // Complex instructions.
    std::shared_ptr<inter::IfInstr> checkIfStatement(inter::ScopePrototype & scopePrototype, syntax::IfStatement & statement);
    std::shared_ptr<inter::WhileInstr> checkWhileStatement(inter::ScopePrototype & scopePrototype, syntax::WhileStatement & statement);

    /* Members */
    syntax::Program * _syntaxTree;
    std::unordered_map<std::string, std::shared_ptr<inter::Function>> _definedFunctions;

    // Success flag.
    bool _processSucceeded = true;
};

#endif // __SEMANTIC_CHECKER_H__
