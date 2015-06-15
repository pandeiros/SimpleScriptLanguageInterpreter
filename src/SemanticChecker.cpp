#include "SemanticChecker.h"

#include "MessageHandler.h"
#include "Library.h"

std::vector<std::shared_ptr<inter::Function>> SemanticChecker::checkAll(const std::shared_ptr<syntax::Program> & program)
{
    // Do some preliminary checks.
    _syntaxTree = program.get();
    _definedFunctions.clear();

    if (!this->scanFunctionDefinitions())
        return std::vector<std::shared_ptr<inter::Function>>();     // Return empty vectors in case of an error.

    if (!this->checkForProgram())
        return std::vector<std::shared_ptr<inter::Function>>();

    return this->traverseAll();
}

bool SemanticChecker::scanFunctionDefinitions()
{
    for (auto & function : _syntaxTree->_functions)
    {
        // Function cannot redefine Library functions.
        if (Library::hasFunction(function->_name))
        {
            MessageHandler::error(std::string("Redefinition of Library function: ").append(function->_name));
            return false;
        }

        // Functions have to be unique, no duplicates.
        if (_definedFunctions.count(function->_name) == 1)
        {
            MessageHandler::error(std::string("Duplicated definition of function: ").append(function->_name));
            return false;
        }

        // Insert new function.
        std::pair<std::string, std::shared_ptr<inter::Function> > newPair(function->_name, std::make_shared<inter::Function>());
        _definedFunctions.insert(newPair);

        // Prepare function.
        auto & exeFunction = _definedFunctions.at(function->_name);
        exeFunction->_name = function->_name;
        exeFunction->_isMain = function->_isMain;

        // Parse variables. check for duplicates.
        for (unsigned int i = 0; i < function->_names.size(); ++i)
            //for (auto & variable : function->names)
        {
            if (!exeFunction->_scopePrototype.addVariable(function->_types[i], function->_names[i]))
            {
                MessageHandler::error(std::string("Duplicated definition of parameter \"")
                                      .append(function->_names[i]).append("\" of function \"").append(function->_name).append("\""));
                return false;
            }
            exeFunction->_scopePrototype.setVariableDefined(function->_names[i]);
        }
    }

    return true;
}

bool SemanticChecker::checkForProgram()
{
    // Check for main "program" function.
    for (auto & function : _definedFunctions)
    {
        if (function.second->_isMain)
            return true;
    }

    MessageHandler::error(std::string("No entry point (a.k.a. \"program\" function) defined"));
    return false;
}

std::vector<std::shared_ptr<inter::Function>> SemanticChecker::traverseAll()
{
    std::vector<std::shared_ptr<inter::Function>> functions;

    // Scan all functions acquired from Parser.
    for (auto & function : _syntaxTree->_functions)
    {
        functions.push_back(this->checkFunction(*function));
    }

    return functions;
}

void SemanticChecker::typeFail(const std::string & type, syntax::Node::Type nodeType)
{
    std::string value = "";
    typedef syntax::Node::Type T;
    switch (nodeType)
    {
        case T::Bool:
            value = "bool";
            break;
        case T::String:
            value = "string";
            break;
        case T::Number:
            value = "int/float";
            break;
    }
    MessageHandler::error(std::string("Type-Value mismatch: cannot assign  <").append(value)
                          .append(">  to  <").append(type).append(">."));
    FAIL;
}

/* Other check functions. */

std::shared_ptr<inter::Function> SemanticChecker::checkFunction(syntax::FunctionDefinition & functionDef)
{
    auto & function = _definedFunctions.at(functionDef._name);

    // Insert block of instructions to the function.
    function->_instructions.push_back(this->checkBlock(function->_scopePrototype, *(functionDef._block)));

    return function;
}

std::shared_ptr<inter::Block> SemanticChecker::checkBlock(inter::ScopePrototype & scopePrototype, syntax::StatementBlock & syntaxBlock)
{
    CHECK_FAIL(nullptr);

    std::shared_ptr<inter::Block> block = std::make_shared<inter::Block>();

    // Set upper scope to this given.
    block->_scopePrototype._upperScope = &scopePrototype;

    // Check every instruction.
    for (auto & instruction : syntaxBlock._instructions)
    {
        CHECK_FAIL(nullptr);

        switch (instruction->getType())
        {
            case syntax::Node::Type::VarDeclaration:
            {
                auto node = static_cast<syntax::VarDeclaration*>(instruction.get());
                this->checkVarDeclaration(block->_scopePrototype, node->_type, node->_name);

                // If variable was assigned, push its value as well.
                if (node->_value)
                {
                    block->_instructions.push_back(this->checkAssignment(block->_scopePrototype, node->_name, *(node->_value)));
                }
                break;
            }
            case syntax::Node::Type::ConstDeclaration:
            {
                auto node = static_cast<syntax::ConstDeclaration*>(instruction.get());
                this->checkConstDeclaration(block->_scopePrototype, node->_type, node->_name);

                // If constant was assigned, push its value as well.
                if (node->_value)
                {
                    block->_instructions.push_back(this->checkAssignment(block->_scopePrototype, node->_name, *(node->_value)));
                }
                break;
            }
            case syntax::Node::Type::Assignment:
            {
                auto node = static_cast<syntax::Assignment*>(instruction.get());

                // Push assignment (checks assignment to constant).
                block->_instructions.push_back(this->checkAssignment(block->_scopePrototype, node->_variable, *(node->_value)));
                break;
            }
            case syntax::Node::Type::ReturnStatement:
            {
                auto node = static_cast<syntax::ReturnStatement*>(instruction.get());

                // Return with a value.
                if (!node->_isEmpty)
                    block->_instructions.push_back(this->checkReturnStatement(block->_scopePrototype, *(node->_value)));
                // Empty return.
                else
                {
                    auto & ret = std::make_shared<inter::ReturnInstr>();
                    ret->_isEmpty = true;
                    block->_instructions.push_back(ret);
                }

                break;
            }
            case syntax::Node::Type::Call:
            {
                auto node = static_cast<syntax::Call*>(instruction.get());
                block->_instructions.push_back(this->checkFunctionCall(block->_scopePrototype, *node));
                break;
            }
            case syntax::Node::Type::StatementBlock:
            {
                auto node = static_cast<syntax::StatementBlock*>(instruction.get());
                block->_instructions.push_back(this->checkBlock(block->_scopePrototype, *node));
                break;
            }
            case syntax::Node::Type::IfStatement:
            {
                auto node = static_cast<syntax::IfStatement*>(instruction.get());
                block->_instructions.push_back(this->checkIfStatement(block->_scopePrototype, *node));
                break;
            }
            case syntax::Node::Type::WhileStatement:
            {
                auto node = static_cast<syntax::WhileStatement*>(instruction.get());
                block->_instructions.push_back(this->checkWhileStatement(block->_scopePrototype, *node));
                break;
            }
            default:
            {
                MessageHandler::error(std::string("Invalid instruction."));
                FAIL;
                return nullptr;
            }
        }
    }

    return block;
}



void SemanticChecker::checkVarDeclaration(inter::ScopePrototype & scopePrototype, const std::string & type, const std::string & name)
{
    CHECK_FAIL();

    if (!scopePrototype.addVariable(type, name))
    {
        MessageHandler::error(std::string("Variable redeclared:  ").append(name));
        FAIL;
    }
}

void SemanticChecker::checkConstDeclaration(inter::ScopePrototype & scopePrototype, const std::string & type, const std::string & name)
{
    CHECK_FAIL();

    if (!scopePrototype.addVariable(type, name, true))
    {
        MessageHandler::error(std::string("Constant redeclared:  ").append(name));
        FAIL;
    }
}

std::shared_ptr<inter::AssignmentInstr> SemanticChecker::checkAssignment(inter::ScopePrototype & scopePrototype, const std::string & variable, syntax::RValue & rvalue)
{
    CHECK_FAIL(nullptr);

    std::shared_ptr<inter::AssignmentInstr> node = std::make_shared<inter::AssignmentInstr>();

    // Check whether variable is declared in scope.
    if (!scopePrototype.hasVariable(variable))
    {
        MessageHandler::error(std::string("Cannot assign to undeclared variable: ").append(variable));
        FAIL;
        return nullptr;
    }

    // Assign and set its value as defined.
    node->_variable->_name = variable;
    node->_value = this->checkAssignable(scopePrototype, rvalue, scopePrototype.getType(variable));
    scopePrototype.setVariableDefined(variable);

    return node;
}

std::shared_ptr<inter::AssignmentInstr> SemanticChecker::checkAssignment(inter::ScopePrototype & scopePrototype, std::shared_ptr<syntax::Variable> & variable, syntax::RValue & rvalue)
{
    CHECK_FAIL(nullptr);

    std::shared_ptr<inter::AssignmentInstr> node = std::make_shared<inter::AssignmentInstr>();

    // Check whether variable is declared in scope.
    if (!scopePrototype.hasVariable(variable->_name))
    {
        MessageHandler::error(std::string("Cannot assign to undeclared variable: ").append(variable->_name));
        FAIL;
        return nullptr;
    }

    // Check for constant.
    if (scopePrototype.isConstant(variable->_name))
    {
        MessageHandler::error(std::string("Cannot assign to constant: ").append(variable->_name));
        FAIL;
        return nullptr;
    }

    node->_variable->_name = variable->_name;
    node->_value = this->checkAssignable(scopePrototype, rvalue, scopePrototype.getType(variable->_name));
    scopePrototype.setVariableDefined(variable->_name);

    return node;
}

std::shared_ptr<inter::Assignable> SemanticChecker::checkAssignable(inter::ScopePrototype & scopePrototype, syntax::RValue & rvalue,
                                                                    const std::string & type)
{
    CHECK_FAIL(nullptr);

    if (rvalue.getType() == syntax::Node::Type::Call)
    {
        return this->checkFunctionCall(scopePrototype, *(static_cast<syntax::Call*>(&rvalue)));
    }
    else if (rvalue.getType() == syntax::Node::Type::ArithmeticExpression)
    {
        return this->checkArithmeticExpression(scopePrototype, *(static_cast<syntax::ArithmeticExpression*>(&rvalue)));
    }
    else if (rvalue.getType() == syntax::Node::Type::LogicalExpression)
    {
        return this->checkLogicalExpression(scopePrototype, *(static_cast<syntax::LogicalExpression*>(&rvalue)));
    }
    else
    {
        typedef syntax::Node::Type Type;
        Type nodeType = rvalue.getType();
        if (nodeType == Type::String || nodeType == Type::Number || nodeType == Type::Bool)
            return this->checkLiteral(scopePrototype, *(static_cast<syntax::Literal*>(&rvalue)), type);
    }

    MessageHandler::error(std::string("Invalid RValue assignment."));
    FAIL;
    return nullptr;
}

std::shared_ptr<inter::CallInstr> SemanticChecker::checkFunctionCall(inter::ScopePrototype & scopePrototype, syntax::Call & function)
{
    CHECK_FAIL(nullptr);

    // Check if functions was defined by user or is defined within library.
    if (_definedFunctions.count(function._name) == 0 && !Library::hasFunction(function._name))
    {
        MessageHandler::error(std::string("Undefined function call: ").append(function._name));
        FAIL;
        return nullptr;
    }

    // Check, whether all function arguments were given.
    if (_definedFunctions.count(function._name) == 1)
    {
        auto & functionDef = _definedFunctions.at(function._name);
        if (functionDef->_scopePrototype._variables.size() != function._arguments.size())
        {
            MessageHandler::error(std::string("Invalid number of arguments for user function '").append(function._name)
                                  .append("'. Expected ").append(std::to_string(functionDef->_scopePrototype._variables.size()))
                                  .append(" arguments, but received ").append(std::to_string(function._arguments.size())).append("."));
            FAIL;
            return nullptr;
        }
    }
    // Function not defined by user, so it has to be library function.
    else
    {
        unsigned int requiredArgs = Library::getFunctionParamsCount(function._name);
        if (requiredArgs != function._arguments.size())
        {
            MessageHandler::error(std::string("Invalid number of arguments for library function '").append(function._name)
                                  .append("'. Expected ").append(std::to_string(requiredArgs)).
                                  append(" arguments, but received ").append(std::to_string(function._arguments.size())));
            FAIL;
            return nullptr;
        }
    }

    std::shared_ptr<inter::CallInstr> obj = std::make_shared<inter::CallInstr>();

    // Set function call name and its arguments.
    obj->name = function._name;
    for (auto & argument : function._arguments)
    {
        obj->arguments.push_back(this->checkAssignable(scopePrototype, *argument));
    }

    return obj;
}

std::shared_ptr<inter::ReturnInstr> SemanticChecker::checkReturnStatement(inter::ScopePrototype & scopePrototype, syntax::RValue & rvalue)
{
    CHECK_FAIL(nullptr);

    std::shared_ptr<inter::ReturnInstr> obj = std::make_shared<inter::ReturnInstr>();

    obj->_value = this->checkAssignable(scopePrototype, rvalue);

    return obj;
}



std::shared_ptr<inter::ArithmeticExpression> SemanticChecker::checkArithmeticExpression(inter::ScopePrototype & scopePrototype, syntax::ArithmeticExpression & expression)
{
    CHECK_FAIL(nullptr);

    std::shared_ptr<inter::ArithmeticExpression> obj = std::make_shared<inter::ArithmeticExpression>();

    obj->operations = expression._operations;

    for (auto & operand : expression._operands)
    {
        if (operand->getType() == syntax::Node::Type::ArithmeticExpression)
        {
            obj->operands.push_back(this->checkArithmeticExpression(scopePrototype, *(static_cast<syntax::ArithmeticExpression*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::Variable)
        {
            obj->operands.push_back(this->checkVariable(scopePrototype, *(static_cast<syntax::Variable*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::Call)
        {
            obj->operands.push_back(this->checkFunctionCall(scopePrototype, *(static_cast<syntax::Call*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::Number)
        {
            obj->operands.push_back(this->checkLiteral(scopePrototype, *(static_cast<syntax::Literal*>(operand.get())), "float"));
        }
        else
        {
            MessageHandler::error(std::string("Invalid arithmetical expression operand!"));
            FAIL;
            return nullptr;
        }
    }

    return obj;
}

std::shared_ptr<inter::LogicalExpression> SemanticChecker::checkLogicalExpression(inter::ScopePrototype & scopePrototype, syntax::LogicalExpression & condition)
{
    CHECK_FAIL(nullptr);

    std::shared_ptr<inter::LogicalExpression> obj = std::make_shared<inter::LogicalExpression>();

    obj->operation = condition._operation;
    obj->negated = condition._isNegated;

    for (auto & operand : condition._operands)
    {
        if (operand->getType() == syntax::Node::Type::LogicalExpression)
        {
            obj->operands.push_back(checkLogicalExpression(scopePrototype, *(static_cast<syntax::LogicalExpression*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::Variable)
        {
            obj->operands.push_back(checkVariable(scopePrototype, *(static_cast<syntax::Variable*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::Call)
        {
            obj->operands.push_back(checkFunctionCall(scopePrototype, *(static_cast<syntax::Call*>(operand.get()))));
        }
        else if (operand->getType() == syntax::Node::Type::ArithmeticExpression)
        {
            obj->operands.push_back(checkArithmeticExpression(scopePrototype, *(static_cast<syntax::ArithmeticExpression*>(operand.get()))));
        }
        else
        {
            typedef syntax::Node::Type Type;
            Type nodeType = operand->getType();
            if (nodeType == Type::Number)
                obj->operands.push_back(this->checkLiteral(scopePrototype, *(static_cast<syntax::Literal*>(operand.get())), "float"));
            else if (nodeType == Type::Bool)
                obj->operands.push_back(this->checkLiteral(scopePrototype, *(static_cast<syntax::Literal*>(operand.get())), "bool"));
            else
            {
                MessageHandler::error(std::string("Invalid logical expression operand!"));
                FAIL;
                return nullptr;
            }
        }
    }

    return obj;
}



std::shared_ptr<inter::Variable> SemanticChecker::checkVariable(inter::ScopePrototype & scopePrototype, syntax::Variable & variable)
{
    CHECK_FAIL(nullptr);

    std::shared_ptr<inter::Variable> obj = std::make_shared<inter::Variable>();
    obj->_name = variable._name;

    // Variable has to be declared.
    if (!scopePrototype.hasVariable(variable._name))
    {
        MessageHandler::error(std::string("Attempt to use undeclared variable: ").append(variable._name));
        return nullptr;
    }

    // Variable has to be defined as well;
    if (!scopePrototype.isVariableDefined(variable._name))
    {
        MessageHandler::error(std::string("Variable not defined before first use: ").append(variable._name));
        return nullptr;
    }

    return obj;
}

std::shared_ptr<inter::Literal> SemanticChecker::checkLiteral(inter::ScopePrototype & scopePrototype,
                                                              syntax::Literal & lit, const std::string & type)
{
    CHECK_FAIL(nullptr);

    std::shared_ptr<inter::Literal> obj = std::make_shared<inter::Literal>();

    typedef syntax::Node::Type Type;
    Type nodeType = lit.getType();

    // Variable type.
    if (type == "bool")
    {
        obj->_type = "bool";

        // Literal type.
        if (nodeType == Type::Bool)
        {
            obj->_boolValue = dynamic_cast<syntax::Bool*>(&lit)->_value;
        }
        else
        {
            this->typeFail(type, nodeType);
            return nullptr;
        }
    }
    else if (type == "int")
    {
        obj->_type = "int";

        if (nodeType == Type::Bool)
        {
            obj->_intValue = dynamic_cast<syntax::Bool*>(&lit)->_value == true ? 1 : 0;
        }
        else if (nodeType == Type::Number)
        {
            syntax::Number * numberNode = dynamic_cast<syntax::Number*>(&lit);
            if (numberNode->_isInteger)
            {
                obj->_intValue = static_cast<int>(numberNode->_value);
            }
            else
            {
                this->typeFail(type, nodeType);
                return nullptr;
            }
        }
        else
        {
            this->typeFail(type, nodeType);
            return nullptr;
        }
    }
    else if (type == "float")
    {
        obj->_type = "float";

        if (nodeType == Type::Bool)
        {
            obj->_floatValue = dynamic_cast<syntax::Bool*>(&lit)->_value == true ? 1.0 : 0.0;
        }
        else if (nodeType == Type::Number)
        {
            syntax::Number * numberNode = dynamic_cast<syntax::Number*>(&lit);
            obj->_floatValue = numberNode->_value;
        }
        else
        {
            this->typeFail(type, nodeType);
            return nullptr;
        }
    }
    else if (type == "string")
    {
        obj->_type = "string";

        if (nodeType == Type::Bool)
        {
            obj->_stringValue = dynamic_cast<syntax::Bool*>(&lit)->_value == true ? "true" : "false";
        }
        else if (nodeType == Type::Number)
        {
            syntax::Number * numberNode = dynamic_cast<syntax::Number*>(&lit);
            if (numberNode->_isInteger)
            {
                obj->_stringValue = std::to_string(static_cast<int>(numberNode->_value));
            }
            else
            {
                obj->_stringValue = std::to_string(numberNode->_value);
            }
        }
        else if (nodeType == Type::String)
        {
            obj->_stringValue = dynamic_cast<syntax::String*>(&lit)->_value;
        }
        else
        {
            this->typeFail(type, nodeType);
            return nullptr;
        }
    }
    else if (type == "")
    {
        switch (nodeType)
        {
            case Type::Bool:
                obj->_type = "bool";
                obj->_boolValue = dynamic_cast<syntax::Bool*>(&lit)->_value;
                break;
            case Type::Number:
            {
                syntax::Number * numberNode = dynamic_cast<syntax::Number*>(&lit);
                if (numberNode->_isInteger)
                {
                    obj->_type = "int";
                    obj->_intValue = static_cast<int>(numberNode->_value);
                }
                else
                {
                    obj->_type = "float";
                    obj->_floatValue = numberNode->_value;
                }
                break;
            }
            case Type::String:
                obj->_type = "string";
                obj->_stringValue = dynamic_cast<syntax::String*>(&lit)->_value;
                break;
            default:
                obj->_type = "";
        }
    }

    return obj;
}


std::shared_ptr<inter::IfInstr> SemanticChecker::checkIfStatement(inter::ScopePrototype & scopePrototype, syntax::IfStatement & statement)
{
    CHECK_FAIL(nullptr);

    std::shared_ptr<inter::IfInstr> obj = std::make_shared<inter::IfInstr>();

    obj->condition = checkLogicalExpression(scopePrototype, *(statement._condition));

    obj->trueBlock = checkBlock(scopePrototype, *(statement._trueBlock));
    if (statement._falseBlock)
    {
        obj->falseBlock = checkBlock(scopePrototype, *(statement._falseBlock));
    }

    return obj;
}

std::shared_ptr<inter::WhileInstr> SemanticChecker::checkWhileStatement(inter::ScopePrototype & scopePrototype, syntax::WhileStatement & statement)
{
    CHECK_FAIL(nullptr);

    std::shared_ptr<inter::WhileInstr> obj = std::make_shared<inter::WhileInstr>();

    obj->_condition = checkLogicalExpression(scopePrototype, *(statement._condition));
    obj->_block = checkBlock(scopePrototype, *(statement._block));

    return obj;
}