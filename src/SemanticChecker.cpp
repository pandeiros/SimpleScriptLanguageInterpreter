#include "SemanticChecker.h"

#include "MessageHandler.h"
#include "Library.h"

std::vector<std::shared_ptr<inter::Function>> SemanticChecker::check(const std::shared_ptr<syntax::Program>& syntaxTree)
{
    this->syntaxTree = syntaxTree.get();
    this->definedFunctions.clear();

    this->scanFunctionDefinitions();
    this->checkMain();

    return this->traverseTree();
}

void SemanticChecker::scanFunctionDefinitions()
{
    for(auto& functionNode: this->syntaxTree->functions)
    {
        if (Library::hasFunction(functionNode->name))
        {
            MessageHandler::error(
                std::string("Redefinition of Standard Lib function: ")
                    .append(functionNode->name)
            );
            return;
        }

        if (this->definedFunctions.count(functionNode->name) == 1)
        {
            MessageHandler::error(
                std::string("Duplicated definition of function: ")
                    .append(functionNode->name)
            );
            return;
        }

        std::pair<std::string, std::shared_ptr<inter::Function> > newPair(functionNode->name, std::make_shared<inter::Function>());
        this->definedFunctions.insert(newPair);
        auto& fun = this->definedFunctions.at(functionNode->name);
        fun->name = functionNode->name;

        for(auto& variable: functionNode->parameters)
        {
            if (!fun->scopeProto.addVariable(variable))
            {
                MessageHandler::error(
                    std::string("Duplicated definition of parameter \"")
                        .append(variable)
                        .append("\" of function \"")
                        .append(functionNode->name)
                        .append("\"")
                );
                return;
            }
            fun->scopeProto.setVariableDefined(variable);
        }
    }
}

void SemanticChecker::checkMain()
{
    if (this->definedFunctions.count("main") == 0)
    {
        MessageHandler::error(
            std::string("No entry point (a.k.a. \"main\" function) defined")
        );
        return;
    }
    if (this->definedFunctions.at("main")->scopeProto.variables.size() != 0)
    {
        MessageHandler::error(
            std::string("\"main\" function should not have parameters")
        );
        return;
    }
}

std::vector<std::shared_ptr<inter::Function>> SemanticChecker::traverseTree()
{
    std::vector<std::shared_ptr<inter::Function>> functions;

    for(auto& function: this->syntaxTree->functions)
    {
        functions.push_back(this->checkFunction(*function));
    }

    return functions;
}

std::shared_ptr<inter::Function> SemanticChecker::checkFunction(syntax::FunDefinition& functionDef)
{
    auto& function = this->definedFunctions.at(functionDef.name);

    function->instructions.push_back(this->checkBlock(function->scopeProto, *(functionDef.blockNode)));

    return function;
}

std::shared_ptr<inter::Block> SemanticChecker::checkBlock(inter::ScopeProto& scopeProto, syntax::StatementBlock& blockNode)
{
    std::shared_ptr<inter::Block> block = std::make_shared<inter::Block>();
    block->scopeProto.upperScope = &scopeProto;

    for(auto& instruction: blockNode.instructions)
    {
        switch(instruction->getType())
        {
            case syntax::Node::Type::VarDeclaration:
            {
                auto node = static_cast<syntax::VarDeclaration*>(instruction.get());
                this->checkVarDeclaration(block->scopeProto, node->name);

                if (node->assignableNode)
                {
                    block->instructions.push_back(this->checkAssignment(block->scopeProto, node->name, *(node->assignableNode)));
                }
                break;
            }
            case syntax::Node::Type::Assignment:
            {
                auto node = static_cast<syntax::Assignment*>(instruction.get());
                block->instructions.push_back(this->checkAssignment(block->scopeProto, *(node->variable), *(node->value)));
                break;
            }
            case syntax::Node::Type::ReturnStatement:
            {
                auto node = static_cast<syntax::ReturnStatement*>(instruction.get());
                block->instructions.push_back(this->checkReturnStatement(block->scopeProto, *(node->assignableNode)));
                break;
            }
            case syntax::Node::Type::Call:
            {
                auto node = static_cast<syntax::Call*>(instruction.get());
                block->instructions.push_back(this->checkFunctionCall(block->scopeProto, *node));
                break;
            }
            case syntax::Node::Type::StatementBlock:
            {
                auto node = static_cast<syntax::StatementBlock*>(instruction.get());
                block->instructions.push_back(this->checkBlock(block->scopeProto, *node));
                break;
            }
            case syntax::Node::Type::IfStatement:
            {
                auto node = static_cast<syntax::IfStatement*>(instruction.get());
                block->instructions.push_back(this->checkIfStatement(block->scopeProto, *node));
                break;
            }
            case syntax::Node::Type::WhileStatement:
            {
                auto node = static_cast<syntax::WhileStatement*>(instruction.get());
                block->instructions.push_back(this->checkWhileStatement(block->scopeProto, *node));
                break;
            }
            case syntax::Node::Type::LoopJump:
            {
                auto node = std::make_shared<inter::LoopJumpInstr>();
                node->isBreak = (static_cast<syntax::LoopJump*>(instruction.get()))->isBreak;
                block->instructions.push_back(node);
                break;
            }
            default:
            {
                MessageHandler::error(
                    std::string("Invalid instruction node type")
                );
            }
        }
    }

    return block;
}

void SemanticChecker::checkVarDeclaration(inter::ScopeProto& scopeProto, const std::string& name)
{
    if (!scopeProto.addVariable(name))
    {
        MessageHandler::error(
            std::string("Redeclaration of variable: ")
                .append(name)
        );
    }
}

std::shared_ptr<inter::AssignmentInstr> SemanticChecker::checkAssignment(inter::ScopeProto& scopeProto, const std::string& variable, syntax::Assignable& assignable)
{
    std::shared_ptr<inter::AssignmentInstr> node = std::make_shared<inter::AssignmentInstr>();

    if (!scopeProto.hasVariable(variable))
    {
        MessageHandler::error(
            std::string("Assignment to undefined variable: ")
                .append(variable)
        );

        return nullptr;
    }

    node->variable->name = variable;
    node->value = this->checkAssignable(scopeProto, assignable);

    scopeProto.setVariableDefined(variable);

    return node;
}


std::shared_ptr<inter::AssignmentInstr> SemanticChecker::checkAssignment(inter::ScopeProto& scopeProto, syntax::Variable& variable, syntax::Assignable& assignable)
{
    std::shared_ptr<inter::AssignmentInstr> node = std::make_shared<inter::AssignmentInstr>();

    if (!scopeProto.hasVariable(variable.variableName))
    {
        MessageHandler::error(
            std::string("Assignment to undefined variable: ")
                .append(variable.variableName)
        );

        return nullptr;
    }

    if (variable.indexArg1 && !scopeProto.isVariableDefined(variable.variableName))
    {
        MessageHandler::error(
            std::string("Indexed assignment to empty variable: ")
                .append(variable.variableName)
        );

        return nullptr;
    }

    node->variable->name = variable.variableName;
    if (variable.indexArg1)
    {
        if (variable.indexArg1->getType() == syntax::Node::Type::Call)
        {
            node->variable->indexArg1 = this->checkFunctionCall(scopeProto, *(static_cast<syntax::Call*>(variable.indexArg1.get())));
        }
        else if (variable.indexArg1->getType() == syntax::Node::Type::Expression)
        {
            node->variable->indexArg1 = this->checkExpression(scopeProto, *(static_cast<syntax::Expression*>(variable.indexArg1.get())));
        }
        else
        {
            MessageHandler::error(
                std::string("Invalid assignable value")
            );
        }
    }
    if (variable.indexArg2)
    {
        if (variable.indexArg2->getType() == syntax::Node::Type::Call)
        {
            node->variable->indexArg2 = this->checkFunctionCall(scopeProto, *(static_cast<syntax::Call*>(variable.indexArg2.get())));
        }
        else if (variable.indexArg2->getType() == syntax::Node::Type::Expression)
        {
            node->variable->indexArg2 = this->checkExpression(scopeProto, *(static_cast<syntax::Expression*>(variable.indexArg2.get())));
        }
        else
        {
            MessageHandler::error(
                std::string("Invalid assignable value")
            );
        }
    }
    node->value = this->checkAssignable(scopeProto, assignable);

    scopeProto.setVariableDefined(variable.variableName);

    return node;
}

std::shared_ptr<inter::Assignable> SemanticChecker::checkAssignable(inter::ScopeProto& scopeProto, syntax::Assignable& assignable)
{
    if (assignable.getType() == syntax::Node::Type::Call)
    {
        return this->checkFunctionCall(scopeProto, *(static_cast<syntax::Call*>(&assignable)));
    }
    else if (assignable.getType() == syntax::Node::Type::Expression)
    {
        return this->checkExpression(scopeProto, *(static_cast<syntax::Expression*>(&assignable)));
    }

    MessageHandler::error(
        std::string("Invalid assignable value")
    );

    return nullptr;
}

std::shared_ptr<inter::Call> SemanticChecker::checkFunctionCall(inter::ScopeProto& scopeProto, syntax::Call& call)
{
    if (this->definedFunctions.count(call.name) == 0 && !Library::hasFunction(call.name))
    {
        MessageHandler::error(
            std::string("Call to undefined function: ")
                .append(call.name)
        );
        return nullptr;
    }

    if (this->definedFunctions.count(call.name) == 1)
    {
        auto &functionDef = this->definedFunctions.at(call.name);
        if (functionDef->scopeProto.variables.size() != call.arguments.size())
        {
            MessageHandler::error(
                std::string("Invalid arguments count for function \"")
                    .append(call.name)
                    .append("\", expected ")
                    .append(std::to_string(functionDef->scopeProto.variables.size()))
                    .append(", got ")
                    .append(std::to_string(call.arguments.size()))
            );
            return nullptr;
        }
    }
    else
    {
        unsigned int requiredArgs = Library::getFunctionParamsCount(call.name);
        if (requiredArgs != call.arguments.size())
        {
            MessageHandler::error(
                std::string("Invalid arguments count for function \"")
                    .append(call.name)
                    .append("\", expected ")
                    .append(std::to_string(requiredArgs))
                    .append(", got ")
                    .append(std::to_string(call.arguments.size()))
            );
            return nullptr;
        }
    }

    std::shared_ptr<inter::Call> obj = std::make_shared<inter::Call>();
    obj->name = call.name;

    for(auto& argument: call.arguments)
    {
        obj->arguments.push_back(this->checkAssignable(scopeProto, *argument));
    }

    return obj;
}

std::shared_ptr<inter::Expression> SemanticChecker::checkExpression(inter::ScopeProto& scopeProto, syntax::Expression& expression)
{
    std::shared_ptr<inter::Expression> obj = std::make_shared<inter::Expression>();

    obj->operations = expression.operations;

    for(auto& operand: expression.operands)
    {
        if (operand->getType() == syntax::Node::Type::Matrix)
        {
            obj->operands.push_back(this->checkMatrixLiteral(*(static_cast<syntax::Matrix*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::Expression)
        {
            obj->operands.push_back(this->checkExpression(scopeProto, *(static_cast<syntax::Expression*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::Variable)
        {
            obj->operands.push_back(this->checkVariable(scopeProto, *(static_cast<syntax::Variable*>(operand.get()))));
        }
        else
        {
            MessageHandler::error(
                std::string("Invalid expression operand")
            );
        }
    }

    return obj;
}

std::shared_ptr<inter::Variable> SemanticChecker::checkVariable(inter::ScopeProto& scopeProto, syntax::Variable& variable)
{
    std::shared_ptr<inter::Variable> obj = std::make_shared<inter::Variable>();

    if (!scopeProto.hasVariable(variable.variableName))
    {
        MessageHandler::error(
            std::string("Usage of undefined variable: ")
                .append(variable.variableName)
        );

        return nullptr;
    }

    if (!scopeProto.isVariableDefined(variable.variableName))
    {
        MessageHandler::error(
            std::string("Usage of empty variable: ")
                .append(variable.variableName)
        );

        return nullptr;
    }

    if (variable.indexArg1 && !scopeProto.isVariableDefined(variable.variableName))
    {
        MessageHandler::error(
            std::string("Indexed usage of empty variable: ")
                .append(variable.variableName)
        );

        return nullptr;
    }

    obj->name = variable.variableName;
    if (variable.indexArg1)
    {
        obj->indexArg1 = this->checkAssignable(scopeProto, *(variable.indexArg1));
    }
    if (variable.indexArg2)
    {
        obj->indexArg2 = this->checkAssignable(scopeProto, *(variable.indexArg2));
    }

    return obj;
}

std::shared_ptr<inter::Return> SemanticChecker::checkReturnStatement(inter::ScopeProto& scopeProto, syntax::Assignable& assignable)
{
    std::shared_ptr<inter::Return> obj = std::make_shared<inter::Return>();

    obj->value = this->checkAssignable(scopeProto, assignable);

    return obj;
}

std::shared_ptr<inter::IfStatementInstr> SemanticChecker::checkIfStatement(inter::ScopeProto& scopeProto, syntax::IfStatement & stmt)
{
    std::shared_ptr<inter::IfStatementInstr> obj = std::make_shared<inter::IfStatementInstr>();

    obj->condition = checkCondition(scopeProto, *(stmt.conditionNode));

    obj->trueBlock =checkBlock(scopeProto, *(stmt.trueBlockNode));
    if (stmt.falseBlockNode)
    {
        obj->falseBlock = checkBlock(scopeProto, *(stmt.falseBlockNode));
    }

    return obj;
}

std::shared_ptr<inter::WhileStatement> SemanticChecker::checkWhileStatement(inter::ScopeProto& scopeProto, syntax::WhileStatement& stmt)
{
    std::shared_ptr<inter::WhileStatement> obj = std::make_shared<inter::WhileStatement>();

    obj->condition = checkCondition(scopeProto, *(stmt.conditionNode));

    obj->block = checkBlock(scopeProto, *(stmt.blockNode));

    return obj;
}

std::shared_ptr<inter::Condition> SemanticChecker::checkCondition(inter::ScopeProto& scopeProto, syntax::Condition& condition)
{
    std::shared_ptr<inter::Condition> obj = std::make_shared<inter::Condition>();

    obj->operation = condition.operation;
    obj->negated = condition.negated;

    for(auto& operand: condition.operands)
    {
        if (operand->getType() == syntax::Node::Type::Matrix)
        {
            obj->operands.push_back(checkMatrixLiteral(*(static_cast<syntax::Matrix*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::Condition)
        {
            obj->operands.push_back(checkCondition(scopeProto, *(static_cast<syntax::Condition*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::Variable)
        {
            obj->operands.push_back(checkVariable(scopeProto, *(static_cast<syntax::Variable*>(operand.get()))));
        }
        else
        {
            MessageHandler::error(
                std::string("Invalid condition operand")
            );
        }
    }

    return obj;
}

std::shared_ptr<inter::Literal> SemanticChecker::checkMatrixLiteral(syntax::Matrix& matrixLiteral)
{
    std::shared_ptr<inter::Literal> obj = std::make_shared<inter::Literal>();

    unsigned int lastRowSize = matrixLiteral.data.at(0).size();

    for(auto it = matrixLiteral.data.begin() + 1; it != matrixLiteral.data.end(); ++it)
    {
        if (it->size() != lastRowSize)
        {
            MessageHandler::error(
                std::string("Invalid matrix literal definition")
            );
        }
    }

    obj->data = matrixLiteral.data;

    return obj;
}
