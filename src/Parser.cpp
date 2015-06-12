#include "Parser.h"

#include <iostream>
#include <limits>
#include "MessageHandler.h"
#include "Lexer.h"

std::shared_ptr<syntax::Program> Parser::parse()
{
    this->resetPreviousToken();

    // Main Program token and its functions.
    Token token;
    std::shared_ptr<syntax::Program> program = std::make_shared<syntax::Program>();
    std::shared_ptr<syntax::FunctionDefinition> function;

    // Parse all available functions.
    while ((function = this->parseFunction()))
    {
        program->addFunction(function);
    }

    return program;
}

/* Parser tools for managing received tokens. */

bool Parser::isAcceptable(const Token & token, const std::initializer_list<TokenType> & acceptableTokens) const
{
    // Iterate through acceptable tokens to check.
    for (auto & acceptable : acceptableTokens)
    {
        if (token._type == acceptable)
        {
            return true;
        }
    }

    return false;
}

Token Parser::accept(const std::initializer_list<TokenType> & acceptableTokens)
{
    Token token;

    // Check for previous tokens.
    if (this->hasBufferedToken())
    {
        token = this->_previousToken;
        this->resetPreviousToken();
    }
    else
        // Or get a new one.
    {
        token = this->_lexer.nextToken();
    }

    // If accepted, return it. Otherwise print error message.
    if (this->isAcceptable(token, acceptableTokens))
    {
        return token;
    }
    else
    {
        PRINT_UNEXP;
        FAIL;
        return token;
    }
}

bool Parser::peek(const std::initializer_list<TokenType> & acceptableTokens)
{
    // Only check, do not accept it yet.

    if (!this->hasBufferedToken())
    {
        this->_previousToken = this->_lexer.nextToken();
    }

    return this->isAcceptable(this->_previousToken, acceptableTokens);
}

Token Parser::getPeeked()
{
    if (!this->hasBufferedToken())
    {
        MessageHandler::debug(std::string("Nothing to peek!"));
    }

    return this->_previousToken;
}

void Parser::peekFail()
{
    Token & token = this->_previousToken;

    PRINT_UNEXP;
    FAIL;
}

const std::string Parser::getErrorIndicator(const unsigned int & pos)
{
    return std::string(pos, ' ').append("^");
}

bool Parser::hasBufferedToken() const
{
    return this->_previousToken._type != TokenType::Undefined;
}

void Parser::resetPreviousToken()
{
    this->_previousToken._type = TokenType::Undefined;
    this->_previousToken._value = "";
    this->_previousToken._line = 0;
    this->_previousToken._col = 0;
}


/* Decomposition procedures */

std::shared_ptr<syntax::FunctionDefinition> Parser::parseFunction()
{
    std::shared_ptr<syntax::FunctionDefinition> node = std::make_shared<syntax::FunctionDefinition>();

    // Main or other function keyword.
    auto token = this->accept({TokenType::Program, TokenType::Function, TokenType::EndOfFile});
    if (token._type == TokenType::EndOfFile)
    {
        return nullptr;
    }

    // Function name.
    auto idToken = this->accept({TokenType::Name});
    if (token._type == TokenType::Program)
        node->setName("program");

    // Parse function arguments.
    std::vector<std::string> types;
    std::vector<std::string> names;
    if (this->parseParameters(types, names))
        node->setParameters(types, names);

    // Link Block with instructions to function.
    node->setBlock(this->parseStatementBlock());

    return node;
}

bool Parser::parseParameters(std::vector<std::string> & types, std::vector<std::string> & names)
{
    Token token;

    // Empty arg list or get type.
    this->accept({TokenType::ParenthOpen});
    token = this->accept({TokenType::ParenthClose, TokenType::Integer, TokenType::String,
                         TokenType::Bool, TokenType::Float});

    // Error.
    if (token._type == TokenType::Undefined)
        return false;

    // At leat one argument.
    if (token._type != TokenType::ParenthClose)
    {
        // Get argument name and save it together with type.
        if (this->peek({TokenType::Name}))
        {
            types.push_back(token._value);
            token = this->accept({TokenType::Name});
            names.push_back(token._value);
        }
        else
            return false;

        // Try to get more arguments.
        while (true)
        {
            // End of list or argument separator.
            token = this->accept({TokenType::ParenthClose, TokenType::Comma});

            // Error.
            if (token._type == TokenType::Undefined)
                return false;

            // End of list.
            if (token._type == TokenType::ParenthClose)
            {
                break;
            }

            // Acquire next argument.
            token = this->accept({TokenType::Integer, TokenType::String,
                                 TokenType::Bool, TokenType::Float});
            if (this->peek({TokenType::Name}))
            {
                types.push_back(token._value);
                token = this->accept({TokenType::Name});
                names.push_back(token._value);
            }
            else
                return false;
        }
    }

    return true;
}

std::shared_ptr<syntax::StatementBlock> Parser::parseStatementBlock()
{
    std::shared_ptr<syntax::StatementBlock> node = std::make_shared<syntax::StatementBlock>();
    Token tempToken;

    // Beginning of a block
    this->accept({TokenType::BracketOpen});

    // Check for instructions, declarations, assignments or line comments.
    while (true)
    {
        if (!this->peek({TokenType::If, TokenType::While, TokenType::Return,
            TokenType::Var, TokenType::Const, TokenType::BracketOpen,
            TokenType::Name, TokenType::Comment}))
        {
            break;
        }

        tempToken = this->getPeeked();

        switch (tempToken._type)
        {
            case TokenType::If:
                node->addInstruction(parseIfStatement());
                break;
            case TokenType::While:
                node->addInstruction(parseWhileStatement());
                break;
            case TokenType::Return:
                node->addInstruction(parseReturnStatement());
                break;
            case TokenType::Var:
                node->addInstruction(parseVarDeclaration());
                break;
            case TokenType::Const:
                node->addInstruction(parseConstDeclaration());
                break;
            case TokenType::BracketOpen:
                node->addInstruction(parseStatementBlock());
                break;
            case TokenType::Name:
                node->addInstruction(parseAssignmentOrFunCall());
                break;
            default:
                break;
        }
    }

    // End of block.
    this->accept({TokenType::BracketClose});

    return node;
}



std::shared_ptr<syntax::ReturnStatement> Parser::parseReturnStatement()
{
    std::shared_ptr<syntax::ReturnStatement> node = std::make_shared<syntax::ReturnStatement>();
    this->accept({TokenType::Return});

    // Empty return.
    if (this->peek({TokenType::Semicolon}))
    {
        this->accept({TokenType::Semicolon});
        node->isEmpty = true;
        return node;
    }

    // Return with a RValue.
    node->setValue(this->parseAssignable());
    this->accept({TokenType::Semicolon});

    return node;
}

std::shared_ptr<syntax::VarDeclaration> Parser::parseVarDeclaration()
{
    std::shared_ptr<syntax::VarDeclaration> node = std::make_shared<syntax::VarDeclaration>();
    this->accept({TokenType::Var});

    // Check for type.
    auto typeToken = this->accept({TokenType::Integer, TokenType::String,
                                  TokenType::Bool, TokenType::Float});

    // Get the variable name.
    auto nameToken = this->accept({TokenType::Name});
    node->setName(nameToken._value);
    node->setType(typeToken._value);

    // If there is sth to assign, do it.
    if (this->peek({TokenType::Assignment}))
    {
        this->accept({TokenType::Assignment});

        node->setValue(this->parseAssignable());
    }

    // Finish assign or empty declaration with a semicolon.
    this->accept({TokenType::Semicolon});

    return node;
}

std::shared_ptr<syntax::ConstDeclaration> Parser::parseConstDeclaration()
{
    std::shared_ptr<syntax::ConstDeclaration> node = std::make_shared<syntax::ConstDeclaration>();
    this->accept({TokenType::Const});

    // Check for type.
    auto typeToken = this->accept({TokenType::Integer, TokenType::String,
                                  TokenType::Bool, TokenType::Float});

    // Get the constant name.
    auto nameToken = this->accept({TokenType::Name});
    node->setName(nameToken._value);
    node->setType(typeToken._value);

    // If there is sth to assign, do it.
    if (this->peek({TokenType::Assignment}))
    {
        this->accept({TokenType::Assignment});

        node->setValue(this->parseAssignable());
    }

    // Finish assign or empty declaration with a semicolon.
    this->accept({TokenType::Semicolon});

    return node;
}

std::shared_ptr<syntax::Assignable> Parser::parseAssignable()
{
    std::shared_ptr<syntax::Assignable> node;

    // Check for name, which can be function call.
    if (this->peek({TokenType::Name}))
    {
        auto token = this->accept({TokenType::Name});

        // Check for function call.
        node = this->parseFunctionCall(token._value);

        if (!node)
        {
            // Otherwise, has to be expression.
            node = this->parseExpression(token);
        }
    }
    else
    {
        node = this->parseExpression();
    }

    return node;
}

std::shared_ptr<syntax::Call> Parser::parseFunctionCall(const std::string & name)
{
    std::shared_ptr<syntax::Call> node = std::make_shared<syntax::Call>();

    // Call operator "()" required.
    if (!this->peek({TokenType::ParenthOpen}))
    {
        return nullptr;
    }

    node->setName(name);
    this->accept({TokenType::ParenthOpen});

    // Empty call.
    if (this->peek({TokenType::ParenthClose}))
    {
        this->accept({TokenType::ParenthClose});
        return node;
    }

    // Otherwise, parse every argument as assignable.
    while (true)
    {
        node->addArgument(this->parseAssignable());

        if (this->peek({TokenType::ParenthClose}))
        {
            this->accept({TokenType::ParenthClose});
            break;
        }

        // Arguments separated by comma.
        if (this->peek({TokenType::Comma}))
        {
            this->accept({TokenType::Comma});
            continue;
        }

        // Error.
        this->peekFail();
    }

    return node;
}



std::shared_ptr<syntax::Assignable> Parser::parseExpression(const Token & initToken)
{
    std::shared_ptr<syntax::Assignable> node = std::make_shared<syntax::Assignable>();

    // Try parsing arithmetic expression.
    node = this->parseArithmeticExpression(initToken);

    if (!node)
    {
        node = this->parseLogicalExpression(initToken);

        if (!node)
        {
            return nullptr;
            FAIL;
        }
    }

    return node;
}


std::shared_ptr<syntax::Assignable> Parser::parseLogicalExpression(const Token & initToken)
{
    std::shared_ptr<syntax::LogicalExpression> node = std::make_shared<syntax::LogicalExpression>();

    // Check if this is correct logical expression (could be arithmetic).
    std::shared_ptr<syntax::LogicalExpression> operand = this->parseStrongLogicalExpression(initToken);
    if (!operand)
        return nullptr;

    node->addOperand(operand);

    while (this->peek({TokenType::Or}))
    {
        auto operatorToken = this->accept({TokenType::Or});
        if (!_parsingSucceeded)
            return nullptr;

        node->setOperator(TokenType::Or);

        // Check again if this is correct logical expression.
        std::shared_ptr<syntax::LogicalExpression> operand = this->parseStrongLogicalExpression();
        if (!operand)
            return nullptr;

        node->addOperand(operand);
    }

    return node;
}

std::shared_ptr<syntax::Assignable> Parser::parseArithmeticExpression(const Token & initToken)
{
    std::shared_ptr<syntax::ArithmeticExpression> node = std::make_shared<syntax::ArithmeticExpression>();

    // Check if this is correct arithmetic expression (could be logical).
    std::shared_ptr<syntax::ArithmeticExpression> operand = this->parseStrongArithmeticExpression(initToken);
    if (!operand)
        return nullptr;

    node->addOperand(operand);

    // Accept weak arithmetic operators.
    while (this->peek({TokenType::Plus, TokenType::Minus}))
    {
        auto operatorToken = this->accept({TokenType::Plus, TokenType::Minus});
        if (!_parsingSucceeded)
            return nullptr;
        node->addOperator(operatorToken._type);

        // Check again if this is correct arithmetic expression.
        std::shared_ptr<syntax::ArithmeticExpression> operand = this->parseStrongArithmeticExpression();
        if (!operand)
            return nullptr;

        node->addOperand(operand);
    }

    return node;
}



std::shared_ptr<syntax::ArithmeticExpression> Parser::parseStrongArithmeticExpression(const Token & initToken)
{
    std::shared_ptr<syntax::ArithmeticExpression> node = std::make_shared<syntax::ArithmeticExpression>();

    // Check if this is correct arithmetic operand.
    NodePtr operand = this->parseArithmeticOperand(initToken);
    if (!operand)
        return nullptr;

    node->addOperand(operand);

    // Accept strong arithmetic operators.
    while (this->peek({TokenType::Multiply, TokenType::Divide, TokenType::Modulo}))
    {
        auto operatorToken = this->accept({TokenType::Multiply, TokenType::Divide, TokenType::Modulo});
        if (!_parsingSucceeded)
            return nullptr;
        node->addOperator(operatorToken._type);

        // Check again if this is correct arithmetic operand.
        NodePtr operand = this->parseArithmeticOperand();
        if (!operand)
            return nullptr;

        node->addOperand(operand);
    }

    return node;
}

NodePtr Parser::parseArithmeticOperand(const Token & initToken)
{
    // This happens, when TokenType::Name was received as first token of the expression.
    if (initToken._type != TokenType::Undefined)
    {
        auto node = this->parseVariable(initToken);
        return node;
    }

    // Otherwise, we have to try other options.

    if (this->peek({TokenType::ParenthOpen}))
    {
        // Try arithmetic expression in parenthesis.
        this->accept({TokenType::ParenthOpen});
        auto node = this->parseArithmeticExpression();
        this->accept({TokenType::ParenthClose});

        return node;
    }

    // Or a variable.
    if (this->peek({TokenType::Name}))
    {
        auto node = this->parseVariable();

        return node;
    }

    // Or a number.
    auto node = this->parseNumber();

    return node;
}


std::shared_ptr<syntax::LogicalExpression> Parser::parseStrongLogicalExpression(const Token & initToken)
{
    std::shared_ptr<syntax::LogicalExpression> node = std::make_shared<syntax::LogicalExpression>();

    // Check if this is correct logical operand.
    NodePtr operand = this->parseRelationExpression();
    if (!operand)
        return nullptr;

    node->addOperand(operand);

    while (this->peek({TokenType::And}))
    {
        this->accept({TokenType::And});
        node->setOperator(TokenType::And);

        // Check again if this is correct logical operand.
        NodePtr operand = this->parseRelationExpression();
        if (!operand)
            return nullptr;

        node->addOperand(operand);
    }

    return node;
}

std::shared_ptr<syntax::LogicalExpression> Parser::parseRelationExpression(const Token & initToken)
{
    std::shared_ptr<syntax::LogicalExpression> node = std::make_shared<syntax::LogicalExpression>();

    // Check if this is correct logical operand.
    NodePtr operand = this->parseLogicalOperand();
    if (!operand)
        return nullptr;

    node->addOperand(operand);

    if (this->peek({TokenType::Less, TokenType::Greater, TokenType::LessOrEqual,
        TokenType::GreaterOrEqual, TokenType::Equality, TokenType::Inequality}))
    {
        auto operatorToken = this->accept({TokenType::Less, TokenType::Greater, TokenType::LessOrEqual,
                                         TokenType::GreaterOrEqual, TokenType::Equality, TokenType::Inequality});
        node->setOperator(operatorToken._type);

        // Check if this is correct logical operand.
        NodePtr operand = this->parseLogicalOperand();
        if (!operand)
            return nullptr;

        node->addOperand(operand);
    }

    //this->tracer.leave ();
    return node;
}

NodePtr Parser::parseLogicalOperand(const Token & initToken)
{
    std::shared_ptr<syntax::LogicalExpression> node = std::make_shared<syntax::LogicalExpression>();

    // Check for negation operator.
    if (this->peek({TokenType::Negation}))
    {
        this->accept({TokenType::Negation});
        node->setNegated();
    }

    // Otherwise, we have to try other options.

    if (this->peek({TokenType::ParenthOpen}))
    {
        // Try logical expression in parenthesis.
        this->accept({TokenType::ParenthOpen});
        node->addOperand(this->parseLogicalExpression());
        this->accept({TokenType::ParenthClose});
    }
    //// Or any assignable.
    //else
    //{
    //    node->addOperand(this->parseAssignable());
    //}

    // Or a variable
    else if (this->peek({TokenType::Name}))
    {
        node->addOperand(this->parseVariable());
    }
    // Or a literal.
    else
    {
        node->addOperand(this->parseLiteral());
    }

    // TODO Understand this
    if (!node->isNegated())
    {
        return node->getLeftSide();
    }

    return node;
}



std::shared_ptr<syntax::Variable> Parser::parseVariable(const Token& identifierToken)
{
    std::shared_ptr<syntax::Variable> node = std::make_shared<syntax::Variable>();

    if (identifierToken._type != TokenType::Name)
    {
        auto tempToken = this->accept({TokenType::Name});
        node->setName(tempToken._value);
    }
    else
    {
        node->setName(identifierToken._value);
    }

    if (this->peek({TokenType::SquareBracketOpen}))
    {
        this->accept({TokenType::SquareBracketOpen});
        node->setIndexArg(1, this->parseAssignable());
        this->accept({TokenType::SquareBracketClose});

        if (this->peek({TokenType::SquareBracketOpen}))
        {
            this->accept({TokenType::SquareBracketOpen});
            node->setIndexArg(2, this->parseAssignable());
            this->accept({TokenType::SquareBracketClose});
        }
    }

    return node;
}

NodePtr Parser::parseLiteral()
{
    std::shared_ptr<syntax::Literal> node = std::make_shared<syntax::Literal>();

    if (this->peek({TokenType::StringLiteral}))
    {
        node = this->parseString();
    }
    else if (this->peek({TokenType::True, TokenType::False}))
    {
        node = this->parseBool();
    }
    else
        node = this->parseNumber();


    return node;
}

std::shared_ptr<syntax::Literal> Parser::parseString()
{
    auto token = this->accept({TokenType::StringLiteral});

    std::shared_ptr<syntax::String> node = std::make_shared<syntax::String>();
    node->_value = token._value;

    return node;
}

std::shared_ptr<syntax::Literal> Parser::parseBool()
{
    auto token = this->accept({TokenType::True, TokenType::False});

    std::shared_ptr<syntax::Bool> node = std::make_shared<syntax::Bool>();
    node->_value = token._value == "true" ? true : false;

    return node;
}

std::shared_ptr<syntax::Literal> Parser::parseNumber()
{
    std::shared_ptr<syntax::Number> node = std::make_shared<syntax::Number>();

    double value = 0;
    bool negative = false;

    if (this->peek({TokenType::Minus}))
    {
        this->accept({TokenType::Minus});

        negative = true;
    }

    auto tempToken = this->accept({TokenType::NumberLiteral});
    if (tempToken._value != "")
    {
        value = std::stod(tempToken._value);
    }

    if (negative)
    {
        value *= -1;
    }

    node->_value = value;
    return node;
}

std::shared_ptr<syntax::IfStatement> Parser::parseIfStatement()
{
    std::shared_ptr<syntax::IfStatement> node = std::make_shared<syntax::IfStatement>();

    //this->tracer.enter ("Parsing if statement");

    this->accept({TokenType::If});
    this->accept({TokenType::ParenthOpen});

    node->setCondition(std::dynamic_pointer_cast<syntax::LogicalExpression>(this->parseLogicalExpression()));

    this->accept({TokenType::ParenthClose});

    node->setTrueBlock(this->parseStatementBlock());

    if (this->peek({TokenType::Else}))
    {
        this->accept({TokenType::Else});

        node->setFalseBlock(this->parseStatementBlock());
    }

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::WhileStatement> Parser::parseWhileStatement()
{
    std::shared_ptr<syntax::WhileStatement> node = std::make_shared<syntax::WhileStatement>();

    this->accept({TokenType::While});
    this->accept({TokenType::ParenthOpen});

    node->setCondition(std::dynamic_pointer_cast<syntax::LogicalExpression>(this->parseLogicalExpression()));

    this->accept({TokenType::ParenthClose});

    node->setBlock(this->parseStatementBlock());

    return node;
}


NodePtr Parser::parseAssignmentOrFunCall()
{
    NodePtr node;

    //this->tracer.enter ("Parsing assignment or function call");

    auto tempToken = this->accept({TokenType::Name});

    node = this->parseFunctionCall(tempToken._value);
    if (!node)
    {
        std::shared_ptr<syntax::Assignment> assignmentNode = std::make_shared<syntax::Assignment>();

        assignmentNode->setVariable(this->parseVariable(tempToken));

        this->accept({TokenType::Assignment});

        assignmentNode->setValue(this->parseAssignable());

        node = assignmentNode;
    }

    this->accept({TokenType::Semicolon});

    return node;
}