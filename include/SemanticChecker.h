#ifndef __SEMANTIC_CHECKER_H__
#define __SEMANTIC_CHECKER_H__

#include "Parser.h"
#include "IR.h"

/*
 * Checks semantic of given program syntax tree.
 */
class SemanticChecker
{
public:
    // Initiate function checking
    std::vector<std::shared_ptr<inter::Function>> check(const std::shared_ptr<syntax::Program>& program);

    // Success flag getter.
    inline bool getCheckSucceeded()
    {
        return _checkSucceeded;
    }

private:
    // Preliminary checks.
    bool scanFunctionDefinitions();
    bool checkForProgram();

    // Main check function. Traverse all functions.
    std::vector<std::shared_ptr<inter::Function>> traverseTree();

    /* Other check functions. */
    std::shared_ptr<inter::Function> checkFunction(syntax::FunctionDefinition& functionDef);
    std::shared_ptr<inter::Block> checkBlock(inter::ScopePrototype & scopePrototype, syntax::StatementBlock& blockNode);
    std::shared_ptr<inter::AssignmentInstr> checkAssignment(inter::ScopePrototype & scopePrototype, const std::string& variable, syntax::Assignable& assignable);
    std::shared_ptr<inter::AssignmentInstr> checkAssignment(inter::ScopePrototype & scopePrototype, syntax::Variable& variable, syntax::Assignable& assignable);
    std::shared_ptr<inter::Assignable> checkAssignable(inter::ScopePrototype & scopePrototype, syntax::Assignable& assignable);
    std::shared_ptr<inter::CallInstr> checkFunctionCall(inter::ScopePrototype & scopePrototype, syntax::Call& call);
    std::shared_ptr<inter::Expression> checkExpression(inter::ScopePrototype & scopePrototype, syntax::ArithmeticExpression& call);
    std::shared_ptr<inter::Variable> checkVariable(inter::ScopePrototype & scopePrototype, syntax::Variable& variable);
    std::shared_ptr<inter::ReturnInstr> checkReturnStatement(inter::ScopePrototype & scopePrototype, syntax::Assignable& assignable);
    std::shared_ptr<inter::IfInstr> checkIfStatement(inter::ScopePrototype & scopePrototype, syntax::IfStatement& stmt);
    std::shared_ptr<inter::WhileInstr> checkWhileStatement(inter::ScopePrototype & scopePrototype, syntax::WhileStatement& stmt);
    std::shared_ptr<inter::Condition> checkCondition(inter::ScopePrototype & scopePrototype, syntax::LogicalExpression& condition);
    //std::shared_ptr<inter::Literal> checkMatrixLiteral(syntax::Matrix& matrixLiteral);
    void checkVarDeclaration(inter::ScopePrototype & scopePrototype, const std::string & type, const std::string & name);

    // Members.
    syntax::Program * _syntaxTree;
    std::unordered_map<std::string, std::shared_ptr<inter::Function>> _definedFunctions;

    // Success flag.
    bool _checkSucceeded = true;
};

#endif // __SEMANTIC_CHECKER_H__
