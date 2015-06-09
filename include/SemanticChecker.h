#ifndef __SEMANTIC_CHECKER_H__
#define __SEMANTIC_CHECKER_H__

#include "Parser.h"
#include "IR.h"

class SemanticChecker
{
public:
    std::vector<std::shared_ptr<inter::Function>> check(const std::shared_ptr<syntax::Program>& syntaxTree);

private:
    syntax::Program* syntaxTree;
    std::unordered_map<std::string, std::shared_ptr<inter::Function>> definedFunctions;

    void scanFunctionDefinitions();
    void checkMain();
    std::vector<std::shared_ptr<inter::Function>> traverseTree();
    std::shared_ptr<inter::Function> checkFunction(syntax::FunDefinition& functionDef);
    std::shared_ptr<inter::Block> checkBlock(inter::ScopeProto& scopeProto, syntax::StatementBlock& blockNode);

    void checkVarDeclaration(inter::ScopeProto& scopeProto, const std::string& name);
    std::shared_ptr<inter::AssignmentInstr> checkAssignment(inter::ScopeProto& scopeProto, const std::string& variable, syntax::Assignable& assignable);
    std::shared_ptr<inter::AssignmentInstr> checkAssignment(inter::ScopeProto& scopeProto, syntax::Variable& variable, syntax::Assignable& assignable);
    std::shared_ptr<inter::Assignable> checkAssignable(inter::ScopeProto& scopeProto, syntax::Assignable& assignable);
    std::shared_ptr<inter::Call> checkFunctionCall(inter::ScopeProto& scopeProto, syntax::Call& call);
    std::shared_ptr<inter::Expression> checkExpression(inter::ScopeProto& scopeProto, syntax::Expression& call);
    std::shared_ptr<inter::Variable> checkVariable(inter::ScopeProto& scopeProto, syntax::Variable& variable);
    std::shared_ptr<inter::Return> checkReturnStatement(inter::ScopeProto& scopeProto, syntax::Assignable& assignable);
    std::shared_ptr<inter::IfStatementInstr> checkIfStatement(inter::ScopeProto& scopeProto, syntax::IfStatement& stmt);
    std::shared_ptr<inter::WhileStatement> checkWhileStatement(inter::ScopeProto& scopeProto, syntax::WhileStatement& stmt);
    std::shared_ptr<inter::Condition> checkCondition(inter::ScopeProto& scopeProto, syntax::Condition& condition);
    std::shared_ptr<inter::Literal> checkMatrixLiteral(syntax::Matrix& matrixLiteral);
};

#endif // __SEMANTIC_CHECKER_H__
