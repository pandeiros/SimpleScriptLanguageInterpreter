#include "SemanticChecker.h"

#include "MessageHandler.h"
#include "Library.h"

std::vector<std::shared_ptr<inter::Function>> SemanticChecker::check(const std::shared_ptr<syntax::Program> & program)
{
    _syntaxTree = program.get();
    _definedFunctions.clear();
    this->scanFunctionDefinitions();
    this->checkMain();      // TODO change to check for "program"

    return this->traverseTree();
}

void SemanticChecker::scanFunctionDefinitions()
{
    for(auto & function : _syntaxTree->functions)
    {
        // Function cannot redefine Library functions.
        if (Library::hasFunction(function->name))
        {
            MessageHandler::error(std::string("Redefinition of Library function: ").append(function->name));
            return;
        }

        // Functions have to be unique, no duplicates.
        if (this->_definedFunctions.count(function->name) == 1)
        {
            MessageHandler::error(std::string("Duplicated definition of function: ").append(function->name));
            return;
        }

        // Insert ne function.
        std::pair<std::string, std::shared_ptr<inter::Function> > newPair(function->name, std::make_shared<inter::Function>());
        this->_definedFunctions.insert(newPair);

        auto & exeFunction = this->_definedFunctions.at(function->name);
        exeFunction->name = function->name;

        for (auto & variable : function->parameters)
        {
            if (!exeFunction->scopePrototype.addVariable(variable))
            {
                MessageHandler::error(std::string("Duplicated definition of parameter \"")
                        .append(variable).append("\" of function \"").append(function->name).append("\""));
                return;
            }
            exeFunction->scopePrototype.setVariableDefined(variable);
        }
    }
}

void SemanticChecker::checkMain()
{
    if (this->_definedFunctions.count("main") == 0)
    {
        MessageHandler::error(
            std::string("No entry point (a.k.a. \"main\" function) defined")
        );
        return;
    }
    if (this->_definedFunctions.at("main")->scopePrototype.variables.size() != 0)
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

    for(auto& function: this->_syntaxTree->functions)
    {
        functions.push_back(this->checkFunction(*function));
    }

    return functions;
}

std::shared_ptr<inter::Function> SemanticChecker::checkFunction(syntax::FunDefinition& functionDef)
{
    auto& function = this->_definedFunctions.at(functionDef.name);

    function->instructions.push_back(this->checkBlock(function->scopePrototype, *(functionDef.blockNode)));

    return function;
}

std::shared_ptr<inter::Block> SemanticChecker::checkBlock(inter::ScopePrototype & scopePrototype, syntax::StatementBlock& blockNode)
{
    std::shared_ptr<inter::Block> block = std::make_shared<inter::Block>();
    block->scopePrototype.upperScope = &scopePrototype;

    for(auto& instruction: blockNode.instructions)
    {
        switch(instruction->getType())
        {
            case syntax::Node::Type::VarDeclaration:
            {
                auto node = static_cast<syntax::VarDeclaration*>(instruction.get());
                this->checkVarDeclaration(block->scopePrototype, node->name);

                if (node->assignableNode)
                {
                    block->instructions.push_back(this->checkAssignment(block->scopePrototype, node->name, *(node->assignableNode)));
                }
                break;
            }
            case syntax::Node::Type::Assignment:
            {
                auto node = static_cast<syntax::Assignment*>(instruction.get());
                block->instructions.push_back(this->checkAssignment(block->scopePrototype, *(node->variable), *(node->value)));
                break;
            }
            case syntax::Node::Type::ReturnStatement:
            {
                auto node = static_cast<syntax::ReturnStatement*>(instruction.get());
                block->instructions.push_back(this->checkReturnStatement(block->scopePrototype, *(node->assignableNode)));
                break;
            }
            case syntax::Node::Type::Call:
            {
                auto node = static_cast<syntax::Call*>(instruction.get());
                block->instructions.push_back(this->checkFunctionCall(block->scopePrototype, *node));
                break;
            }
            case syntax::Node::Type::StatementBlock:
            {
                auto node = static_cast<syntax::StatementBlock*>(instruction.get());
                block->instructions.push_back(this->checkBlock(block->scopePrototype, *node));
                break;
            }
            case syntax::Node::Type::IfStatement:
            {
                auto node = static_cast<syntax::IfStatement*>(instruction.get());
                block->instructions.push_back(this->checkIfStatement(block->scopePrototype, *node));
                break;
            }
            case syntax::Node::Type::WhileStatement:
            {
                auto node = static_cast<syntax::WhileStatement*>(instruction.get());
                block->instructions.push_back(this->checkWhileStatement(block->scopePrototype, *node));
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

void SemanticChecker::checkVarDeclaration(inter::ScopePrototype & scopePrototype, const std::string& name)
{
    if (!scopePrototype.addVariable(name))
    {
        MessageHandler::error(
            std::string("Redeclaration of variable: ")
                .append(name)
        );
    }
}

std::shared_ptr<inter::AssignmentInstr> SemanticChecker::checkAssignment(inter::ScopePrototype & scopePrototype, const std::string& variable, syntax::Assignable& assignable)
{
    std::shared_ptr<inter::AssignmentInstr> node = std::make_shared<inter::AssignmentInstr>();

    if (!scopePrototype.hasVariable(variable))
    {
        MessageHandler::error(
            std::string("Assignment to undefined variable: ")
                .append(variable)
        );

        return nullptr;
    }

    node->variable->name = variable;
    node->value = this->checkAssignable(scopePrototype, assignable);

    scopePrototype.setVariableDefined(variable);

    return node;
}


std::shared_ptr<inter::AssignmentInstr> SemanticChecker::checkAssignment(inter::ScopePrototype & scopePrototype, syntax::Variable& variable, syntax::Assignable& assignable)
{
    std::shared_ptr<inter::AssignmentInstr> node = std::make_shared<inter::AssignmentInstr>();

    if (!scopePrototype.hasVariable(variable.variableName))
    {
        MessageHandler::error(
            std::string("Assignment to undefined variable: ")
                .append(variable.variableName)
        );

        return nullptr;
    }

    if (variable.indexArg1 && !scopePrototype.isVariableDefined(variable.variableName))
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
            node->variable->indexArg1 = this->checkFunctionCall(scopePrototype, *(static_cast<syntax::Call*>(variable.indexArg1.get())));
        }
        else if (variable.indexArg1->getType() == syntax::Node::Type::Expression)
        {
            node->variable->indexArg1 = this->checkExpression(scopePrototype, *(static_cast<syntax::Expression*>(variable.indexArg1.get())));
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
            node->variable->indexArg2 = this->checkFunctionCall(scopePrototype, *(static_cast<syntax::Call*>(variable.indexArg2.get())));
        }
        else if (variable.indexArg2->getType() == syntax::Node::Type::Expression)
        {
            node->variable->indexArg2 = this->checkExpression(scopePrototype, *(static_cast<syntax::Expression*>(variable.indexArg2.get())));
        }
        else
        {
            MessageHandler::error(
                std::string("Invalid assignable value")
            );
        }
    }
    node->value = this->checkAssignable(scopePrototype, assignable);

    scopePrototype.setVariableDefined(variable.variableName);

    return node;
}

std::shared_ptr<inter::Assignable> SemanticChecker::checkAssignable(inter::ScopePrototype & scopePrototype, syntax::Assignable& assignable)
{
    if (assignable.getType() == syntax::Node::Type::Call)
    {
        return this->checkFunctionCall(scopePrototype, *(static_cast<syntax::Call*>(&assignable)));
    }
    else if (assignable.getType() == syntax::Node::Type::Expression)
    {
        return this->checkExpression(scopePrototype, *(static_cast<syntax::Expression*>(&assignable)));
    }

    MessageHandler::error(
        std::string("Invalid assignable value")
    );

    return nullptr;
}

std::shared_ptr<inter::Call> SemanticChecker::checkFunctionCall(inter::ScopePrototype & scopePrototype, syntax::Call& call)
{
    if (this->_definedFunctions.count(call.name) == 0 && !Library::hasFunction(call.name))
    {
        MessageHandler::error(
            std::string("Call to undefined function: ")
                .append(call.name)
        );
        return nullptr;
    }

    if (this->_definedFunctions.count(call.name) == 1)
    {
        auto &functionDef = this->_definedFunctions.at(call.name);
        if (functionDef->scopePrototype.variables.size() != call.arguments.size())
        {
            MessageHandler::error(
                std::string("Invalid arguments count for function \"")
                    .append(call.name)
                    .append("\", expected ")
                    .append(std::to_string(functionDef->scopePrototype.variables.size()))
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
        obj->arguments.push_back(this->checkAssignable(scopePrototype, *argument));
    }

    return obj;
}

std::shared_ptr<inter::Expression> SemanticChecker::checkExpression(inter::ScopePrototype & scopePrototype, syntax::Expression& expression)
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
            obj->operands.push_back(this->checkExpression(scopePrototype, *(static_cast<syntax::Expression*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::Variable)
        {
            obj->operands.push_back(this->checkVariable(scopePrototype, *(static_cast<syntax::Variable*>(operand.get()))));
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

std::shared_ptr<inter::Variable> SemanticChecker::checkVariable(inter::ScopePrototype & scopePrototype, syntax::Variable& variable)
{
    std::shared_ptr<inter::Variable> obj = std::make_shared<inter::Variable>();

    if (!scopePrototype.hasVariable(variable.variableName))
    {
        MessageHandler::error(
            std::string("Usage of undefined variable: ")
                .append(variable.variableName)
        );

        return nullptr;
    }

    if (!scopePrototype.isVariableDefined(variable.variableName))
    {
        MessageHandler::error(
            std::string("Usage of empty variable: ")
                .append(variable.variableName)
        );

        return nullptr;
    }

    if (variable.indexArg1 && !scopePrototype.isVariableDefined(variable.variableName))
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
        obj->indexArg1 = this->checkAssignable(scopePrototype, *(variable.indexArg1));
    }
    if (variable.indexArg2)
    {
        obj->indexArg2 = this->checkAssignable(scopePrototype, *(variable.indexArg2));
    }

    return obj;
}

std::shared_ptr<inter::Return> SemanticChecker::checkReturnStatement(inter::ScopePrototype & scopePrototype, syntax::Assignable& assignable)
{
    std::shared_ptr<inter::Return> obj = std::make_shared<inter::Return>();

    obj->value = this->checkAssignable(scopePrototype, assignable);

    return obj;
}

std::shared_ptr<inter::IfStatementInstr> SemanticChecker::checkIfStatement(inter::ScopePrototype & scopePrototype, syntax::IfStatement & stmt)
{
    std::shared_ptr<inter::IfStatementInstr> obj = std::make_shared<inter::IfStatementInstr>();

    obj->condition = checkCondition(scopePrototype, *(stmt.conditionNode));

    obj->trueBlock =checkBlock(scopePrototype, *(stmt.trueBlockNode));
    if (stmt.falseBlockNode)
    {
        obj->falseBlock = checkBlock(scopePrototype, *(stmt.falseBlockNode));
    }

    return obj;
}

std::shared_ptr<inter::WhileStatement> SemanticChecker::checkWhileStatement(inter::ScopePrototype & scopePrototype, syntax::WhileStatement& stmt)
{
    std::shared_ptr<inter::WhileStatement> obj = std::make_shared<inter::WhileStatement>();

    obj->condition = checkCondition(scopePrototype, *(stmt.conditionNode));

    obj->block = checkBlock(scopePrototype, *(stmt.blockNode));

    return obj;
}

std::shared_ptr<inter::Condition> SemanticChecker::checkCondition(inter::ScopePrototype & scopePrototype, syntax::Condition& condition)
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
            obj->operands.push_back(checkCondition(scopePrototype, *(static_cast<syntax::Condition*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::Variable)
        {
            obj->operands.push_back(checkVariable(scopePrototype, *(static_cast<syntax::Variable*>(operand.get()))));
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
