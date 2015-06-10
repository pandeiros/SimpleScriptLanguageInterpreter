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

private:
    void scanFunctionDefinitions();
    void checkMain();
    std::vector<std::shared_ptr<inter::Function>> traverseTree();
    std::shared_ptr<inter::Function> checkFunction(syntax::FunDefinition& functionDef);
    std::shared_ptr<inter::Block> checkBlock(inter::ScopePrototype & scopePrototype, syntax::StatementBlock& blockNode);

    void checkVarDeclaration(inter::ScopePrototype & scopePrototype, const std::string& name);
    std::shared_ptr<inter::AssignmentInstr> checkAssignment(inter::ScopePrototype & scopePrototype, const std::string& variable, syntax::Assignable& assignable);
    std::shared_ptr<inter::AssignmentInstr> checkAssignment(inter::ScopePrototype & scopePrototype, syntax::Variable& variable, syntax::Assignable& assignable);
    std::shared_ptr<inter::Assignable> checkAssignable(inter::ScopePrototype & scopePrototype, syntax::Assignable& assignable);
    std::shared_ptr<inter::Call> checkFunctionCall(inter::ScopePrototype & scopePrototype, syntax::Call& call);
    std::shared_ptr<inter::Expression> checkExpression(inter::ScopePrototype & scopePrototype, syntax::Expression& call);
    std::shared_ptr<inter::Variable> checkVariable(inter::ScopePrototype & scopePrototype, syntax::Variable& variable);
    std::shared_ptr<inter::Return> checkReturnStatement(inter::ScopePrototype & scopePrototype, syntax::Assignable& assignable);
    std::shared_ptr<inter::IfStatementInstr> checkIfStatement(inter::ScopePrototype & scopePrototype, syntax::IfStatement& stmt);
    std::shared_ptr<inter::WhileStatement> checkWhileStatement(inter::ScopePrototype & scopePrototype, syntax::WhileStatement& stmt);
    std::shared_ptr<inter::Condition> checkCondition(inter::ScopePrototype & scopePrototype, syntax::Condition& condition);
    std::shared_ptr<inter::Literal> checkMatrixLiteral(syntax::Matrix& matrixLiteral);

    syntax::Program * _syntaxTree;
    std::unordered_map<std::string, std::shared_ptr<inter::Function>> _definedFunctions;
};

#endif // __SEMANTIC_CHECKER_H__
