#include "Parser.h"

#include "MessageHandler.h"
#include "Lexer.h"

#include <iostream>
#include <limits>


Parser::Parser (Lexer& lexer) :
_lexer (lexer)
{}

std::shared_ptr<syntax::Program> Parser::parse ()
{
    //this->tracer.reset ();

    this->resetPreviousToken ();

    Token token;

    std::shared_ptr<syntax::Program> syntaxTree = std::make_shared<syntax::Program> ();
    std::shared_ptr<syntax::FunDefinition> lastFunction;

    //this->tracer.enter ("Starting parser tracing...");
    while ((lastFunction = this->parseFunction ()))
    {
        syntaxTree->addFunction (lastFunction);
    }
    //this->tracer.leave ("Trace ended...");

    return syntaxTree;
}

bool Parser::isAcceptable (const Token& token, const std::initializer_list<TokenType>& acceptable) const
{
    for (auto& it : acceptable)
    {
        if (token._type == it)
        {
            return true;
        }
    }

    return false;
}

Token Parser::accept (const std::initializer_list<TokenType>& acceptable)
{
    Token token;

    if (this->hasBufferedToken ())
    {
        token = this->_previousToken;
        this->resetPreviousToken ();
    }
    else
    {
        token = this->_lexer.nextToken ();
    }

    if (this->isAcceptable (token, acceptable))
    {
        return token;
    }
    else
    {
        MessageHandler::error (
            std::string ("Unexpected token: ")
            .append (getTokenTypeName (token._type))
            .append (" (Line: ")
            .append (std::to_string (token._line))
            .append (", Pos: ")
            .append (std::to_string (token._col))
            .append (")")
            .append ("\n")
            .append (this->_lexer.getLine (token._lineStart))
            .append ("\n")
            .append (this->makeErrorMarker (token._col))
            );

        return token;
    }
}

bool Parser::peek (const std::initializer_list<TokenType>& acceptable)
{
    if (!this->hasBufferedToken ())
    {
        this->_previousToken = this->_lexer.nextToken ();
    }

    return this->isAcceptable (this->_previousToken, acceptable);
}

Token Parser::getPeeked ()
{
    if (!this->hasBufferedToken ())
    {
        MessageHandler::error (std::string ("Nothing peeked"));
    }

    return this->_previousToken;
}

void Parser::peekFail ()
{
    Token& token = this->_previousToken;

    MessageHandler::error (
        std::string ("Unexpected token: ")
        .append (getTokenTypeName (token._type))
        .append (" (Line: ")
        .append (std::to_string (token._line))
        .append (", Pos: ")
        .append (std::to_string (token._col))
        .append (")")
        .append ("\n")
        .append (this->_lexer.getLine (token._lineStart))
        .append ("\n")
        .append (this->makeErrorMarker (token._col))
        );
}

const std::string Parser::makeErrorMarker (const unsigned int& pos)
{
    return std::string (pos, ' ').append ("^");
}

bool Parser::hasBufferedToken () const
{
    return this->_previousToken._type != TokenType::Undefined;
}

void Parser::resetPreviousToken ()
{
    this->_previousToken._type = TokenType::Undefined;
    this->_previousToken._value = "";
    this->_previousToken._line = 0;
    this->_previousToken._col = 0;
}

std::shared_ptr<syntax::FunDefinition> Parser::parseFunction ()
{
    std::shared_ptr<syntax::FunDefinition> node = std::make_shared<syntax::FunDefinition> ();

    //this->tracer.enter ("Parsing function");

    auto tempToken = this->accept ({TokenType::Function, TokenType::EndOfFile});
    if (tempToken._type == TokenType::EndOfFile)
    {
        //this->tracer.leave ("EOF");
        return nullptr;
    }

    tempToken = this->accept ({TokenType::Identifier});
    node->setName (tempToken._value);
    node->setParameters (this->parseParameters ());
    node->setBlock (this->parseStatementBlock ());

    //this->tracer.leave ();
    return node;
}

std::vector<std::string> Parser::parseParameters ()
{
    std::vector<std::string> parametersNames;

    //this->tracer.enter ("Parsing parameters");

    Token tempToken;

    this->accept ({TokenType::ParenthOpen});

    tempToken = this->accept ({TokenType::ParenthClose, TokenType::Identifier});
    if (tempToken._type != TokenType::ParenthClose)
    {
        parametersNames.push_back (tempToken._value);

        while (true)
        {
            tempToken = this->accept ({TokenType::ParenthClose, TokenType::Comma});
            if (tempToken._type == TokenType::ParenthClose)
            {
                break;
            }
            tempToken = this->accept ({TokenType::Identifier});

            parametersNames.push_back (tempToken._value);
        }
    }

    //this->tracer.leave ();
    return parametersNames;
}

std::shared_ptr<syntax::StatementBlock> Parser::parseStatementBlock()
{
    std::shared_ptr<syntax::StatementBlock> node = std::make_shared<syntax::StatementBlock>();

    //this->tracer.enter ("Parsing statement block");

    this->accept ({TokenType::BracketOpen});

    Token tempToken;

    while (true)
    {
        if (!this->peek ({
            TokenType::If,
            TokenType::While,
            TokenType::Return,
            TokenType::Var,
            TokenType::BracketOpen,
            TokenType::Identifier,
            TokenType::Continue,
            TokenType::Break
        }))
        {
            break;
        }

            tempToken = this->getPeeked ();

            switch (tempToken._type)
            {
                case TokenType::If:
                    node->addInstruction (parseIfStatement ());
                    break;
                case TokenType::While:
                    node->addInstruction (parseWhileStatement ());
                    break;
                case TokenType::Return:
                    node->addInstruction (parseReturnStatement ());
                    break;
                case TokenType::Var:
                    node->addInstruction (parseInitStatement ());
                    break;
                case TokenType::BracketOpen:
                    node->addInstruction (parseStatementBlock ());
                    break;
                case TokenType::Identifier:
                    node->addInstruction (parseAssignmentOrFunCall ());
                    break;
                case TokenType::Continue:
                case TokenType::Break:
                    node->addInstruction (parseLoopJump ());
                    break;
                default:
                    break;
            }
    }

    this->accept ({TokenType::BracketClose});

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::IfStatement> Parser::parseIfStatement()
{
    std::shared_ptr<syntax::IfStatement> node = std::make_shared<syntax::IfStatement>();

    //this->tracer.enter ("Parsing if statement");

    this->accept ({TokenType::If});
    this->accept ({TokenType::ParenthOpen});

    node->setCondition (this->parseCondition ());

    this->accept ({TokenType::ParenthClose});

    node->setTrueBlock (this->parseStatementBlock ());

    if (this->peek ({TokenType::Else}))
    {
        this->accept ({TokenType::Else});

        node->setFalseBlock (this->parseStatementBlock ());
    }

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::WhileStatement> Parser::parseWhileStatement()
{
    std::shared_ptr<syntax::WhileStatement> node = std::make_shared<syntax::WhileStatement>();

    //this->tracer.enter ("Parsing while statement");

    this->accept ({TokenType::While});
    this->accept ({TokenType::ParenthOpen});

    node->setCondition (this->parseCondition ());

    this->accept ({TokenType::ParenthClose});

    node->setBlock (this->parseStatementBlock ());

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::ReturnStatement> Parser::parseReturnStatement()
{
    std::shared_ptr<syntax::ReturnStatement> node = std::make_shared<syntax::ReturnStatement>();

    //this->tracer.enter ("Parsing return statement");

    this->accept ({TokenType::Return});

    node->setValue (this->parseAssignable ());

    this->accept ({TokenType::Semicolon});

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::VarDeclaration> Parser::parseInitStatement()
{
    std::shared_ptr<syntax::VarDeclaration> node = std::make_shared<syntax::VarDeclaration>();

    //this->tracer.enter ("Parsing init statement");

    this->accept ({TokenType::Var});

    auto nameToken = this->accept ({TokenType::Identifier});
    node->setName (nameToken._value);

    if (this->peek ({TokenType::Assignment}))
    {
        this->accept ({TokenType::Assignment});

        node->setValue (this->parseAssignable ());
    }

    this->accept ({TokenType::Semicolon});

   // this->tracer.leave ();
    return node;
}

NodePtr Parser::parseAssignmentOrFunCall ()
{
    NodePtr node;

    //this->tracer.enter ("Parsing assignment or function call");

    auto tempToken = this->accept ({TokenType::Identifier});

    node = this->parseFunCall (tempToken._value);
    if (!node)
    {
        std::shared_ptr<syntax::Assignment> assignmentNode = std::make_shared<syntax::Assignment>();

        assignmentNode->setVariable (this->parseVariable (tempToken));

        this->accept ({TokenType::Assignment});

        assignmentNode->setValue (this->parseAssignable ());

        node = assignmentNode;
    }

    this->accept ({TokenType::Semicolon});

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::LoopJump> Parser::parseLoopJump()
{
    std::shared_ptr<syntax::LoopJump> node = std::make_shared<syntax::LoopJump>();

    //this->tracer.enter ("Parsing loop jump");

    auto token = this->accept ({TokenType::Continue, TokenType::Break});
    node->setType (token._type);

    this->accept ({TokenType::Semicolon});

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::Assignable> Parser::parseAssignable()
{
    std::shared_ptr<syntax::Assignable> node;

    //this->tracer.enter ("Parsing assignable");

    if (this->peek ({TokenType::Identifier}))
    {
        auto tempToken = this->accept ({TokenType::Identifier});

        node = this->parseFunCall (tempToken._value);
        if (!node)
        {
            node = this->parseExpression (tempToken);
        }
    }
    else
    {
        node = this->parseExpression ();
    }

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::Call> Parser::parseFunCall(const std::string& identifier)
{
    std::shared_ptr<syntax::Call> node = std::make_shared<syntax::Call>();

    //this->tracer.enter ("Parsing function call");

    if (!this->peek ({TokenType::ParenthOpen}))
    {
        //this->tracer.leave ("  - not a function call");
        return nullptr;
    }

    node->setName (identifier);

    this->accept ({TokenType::ParenthOpen});

    if (this->peek ({TokenType::ParenthClose}))
    {
        this->accept ({TokenType::ParenthClose});

        //this->tracer.leave ("  + function call");
        return node;
    }

    while (true)
    {
        node->addArgument (this->parseAssignable ());

        if (this->peek ({TokenType::ParenthClose}))
        {
            this->accept ({TokenType::ParenthClose});
            break;
        }
        if (this->peek ({TokenType::Comma}))
        {
            this->accept ({TokenType::Comma});
            continue;
        }

        this->peekFail ();
    }

    //this->tracer.leave ("  + function call");
    return node;
}

std::shared_ptr<syntax::Variable> Parser::parseVariable (const Token& identifierToken)
{
    std::shared_ptr<syntax::Variable> node = std::make_shared<syntax::Variable> ();

    //this->tracer.enter ("Parsing Variable");

    if (identifierToken._type != TokenType::Identifier)
    {
        auto tempToken = this->accept ({TokenType::Identifier});
        node->setName (tempToken._value);
    }
    else
    {
        node->setName (identifierToken._value);
    }

    if (this->peek ({TokenType::SquareBracketOpen}))
    {
        this->accept ({TokenType::SquareBracketOpen});
        node->setIndexArg (1, this->parseAssignable ());
        this->accept ({TokenType::SquareBracketClose});

        if (this->peek ({TokenType::SquareBracketOpen}))
        {
            this->accept ({TokenType::SquareBracketOpen});
            node->setIndexArg (2, this->parseAssignable ());
            this->accept ({TokenType::SquareBracketClose});
        }
    }

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::Matrix> Parser::parseLiteral()
{
    std::shared_ptr<syntax::Matrix> node = std::make_shared<syntax::Matrix>();

    ///this->tracer.enter ("Parsing literal");

    if (this->peek ({TokenType::SquareBracketOpen}))
    {
        node = this->parseMatrixLiteral ();
    }
    else
    {
        node->pushValue (this->parseNumberLiteral ());
    }

    //this->tracer.leave ();
    return node;
}

double Parser::parseNumberLiteral ()
{
    double value;
    bool negative = false;

    //this->tracer.enter ("Parsing number literal");

    if (this->peek ({TokenType::Minus}))
    {
        this->accept ({TokenType::Minus});

        negative = true;
    }

    auto tempToken = this->accept ({TokenType::Infinity, TokenType::NumberLiteral});
    if (tempToken._type == TokenType::Infinity)
    {
        value = std::numeric_limits<double>::infinity ();
    }
    else
    {
        value = std::stod (tempToken._value);
    }

    if (negative)
    {
        value *= -1;
    }

    //this->tracer.leave ();
    return value;
}

std::shared_ptr<syntax::Matrix> Parser::parseMatrixLiteral()
{
    std::shared_ptr<syntax::Matrix> node = std::make_shared<syntax::Matrix>();

    //this->tracer.enter ("Parsing matrix literal");

    this->accept ({TokenType::SquareBracketOpen});

    node->pushValue (this->parseNumberLiteral ());

    while (!this->peek ({TokenType::SquareBracketClose}))
    {
        if (this->peek ({TokenType::Comma}))
        {
            this->accept ({TokenType::Comma});

            node->pushValue (this->parseNumberLiteral ());
        }
        else if (this->peek ({TokenType::Semicolon}))
        {
            this->accept ({TokenType::Semicolon});

            node->nextLevel ();

            node->pushValue (this->parseNumberLiteral ());
        }
        else
        {
            this->peekFail ();
        }
    }

    while (!this->peek ({TokenType::SquareBracketClose}))
    {
        while (!this->peek ({TokenType::Semicolon}))
        {
            this->accept ({TokenType::Comma});

            node->pushValue (this->parseNumberLiteral ());
        }
        this->accept ({TokenType::Semicolon});

        node->pushValue (this->parseNumberLiteral ());

        node->nextLevel ();

        node->pushValue (this->parseNumberLiteral ());
    }
    this->accept ({TokenType::SquareBracketClose});

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::Expression> Parser::parseExpression(const Token& firstToken)
{
    std::shared_ptr<syntax::Expression> node = std::make_shared<syntax::Expression>();

    //this->tracer.enter ("Parsing expression");

    node->addOperand (this->parseMultiplicativeExpression (firstToken));

    while (this->peek ({TokenType::Plus, TokenType::Minus}))
    {
        auto tempToken = this->accept ({TokenType::Plus, TokenType::Minus});
        node->addOperator (tempToken._type);

        node->addOperand (this->parseMultiplicativeExpression ());
    }

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::Expression> Parser::parseMultiplicativeExpression(const Token& firstToken)
{
    std::shared_ptr<syntax::Expression> node = std::make_shared<syntax::Expression>();

    //this->tracer.enter ("Parsing multiplicative expression");

    node->addOperand (this->parsePrimaryExpression (firstToken));

    while (this->peek ({TokenType::Multiply, TokenType::Divide, TokenType::Modulo}))
    {
        auto tempToken = this->accept ({TokenType::Multiply, TokenType::Divide, TokenType::Modulo});
        node->addOperator (tempToken._type);

        node->addOperand (this->parsePrimaryExpression ());
    }

    //this->tracer.leave ();
    return node;
}

NodePtr Parser::parsePrimaryExpression (const Token& firstToken)
{
    //this->tracer.enter ("Parsing primary expression");
    //this->tracer.info (std::string ("First Token type = ").append (getTokenTypeName (firstToken._type)));

    if (firstToken._type != TokenType::Undefined)
    {
        auto node = this->parseVariable (firstToken);

        //this->tracer.leave ();
        return node;
    }

    if (this->peek ({TokenType::ParenthOpen}))
    {
        this->accept ({TokenType::ParenthOpen});
        auto node = this->parseExpression ();
        this->accept ({TokenType::ParenthClose});

        //this->tracer.leave ();
        return node;
    }

    if (this->peek ({TokenType::Identifier}))
    {
        auto node = this->parseVariable ();

        //this->tracer.leave ();
        return node;
    }

    auto node = this->parseLiteral ();

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::Condition> Parser::parseCondition()
{
    std::shared_ptr<syntax::Condition> node = std::make_shared<syntax::Condition>();

    //this->tracer.enter ("Parsing condition");

    node->addOperand (this->parseAndCondition ());

    while (this->peek ({TokenType::Or}))
    {
        this->accept ({TokenType::Or});
        node->setOperator (TokenType::Or);

        node->addOperand (this->parseAndCondition ());
    }

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::Condition> Parser::parseAndCondition()
{
    std::shared_ptr<syntax::Condition> node = std::make_shared<syntax::Condition>();

    //this->tracer.enter ("Parsing and condition");

    node->addOperand (this->parseEqualityCondition ());

    while (this->peek ({TokenType::And}))
    {
        this->accept ({TokenType::And});
        node->setOperator (TokenType::And);

        node->addOperand (this->parseEqualityCondition ());
    }

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::Condition> Parser::parseEqualityCondition()
{
    std::shared_ptr<syntax::Condition> node = std::make_shared<syntax::Condition>();

    //this->tracer.enter ("Parsing equality condition");

    node->addOperand (this->parseRelationalCondition ());

    if (this->peek ({TokenType::Equality, TokenType::Inequality}))
    {
        auto tempToken = this->accept ({TokenType::Equality, TokenType::Inequality});
        node->setOperator (tempToken._type);

        node->addOperand (this->parseRelationalCondition ());
    }

    //this->tracer.leave ();
    return node;
}

std::shared_ptr<syntax::Condition> Parser::parseRelationalCondition()
{
    std::shared_ptr<syntax::Condition> node = std::make_shared<syntax::Condition>();

    //this->tracer.enter ("Parsing relational condition");

    node->addOperand (this->parsePrimaryCondition ());

    if (this->peek ({TokenType::Less, TokenType::Greater, TokenType::LessOrEqual, TokenType::GreaterOrEqual}))
    {
        auto tempToken = this->accept ({TokenType::Less, TokenType::Greater, TokenType::LessOrEqual, TokenType::GreaterOrEqual});
        node->setOperator (tempToken._type);

        node->addOperand (this->parsePrimaryCondition ());
    }

    //this->tracer.leave ();
    return node;
}

NodePtr Parser::parsePrimaryCondition ()
{
    std::shared_ptr<syntax::Condition> node = std::make_shared<syntax::Condition>();

    //this->tracer.enter ("Parsing primary condition");

    if (this->peek ({TokenType::Negation}))
    {
        this->accept ({TokenType::Negation});

        node->setNegated ();
    }

    if (this->peek ({TokenType::ParenthOpen}))
    {
        this->accept ({TokenType::ParenthOpen});
        node->addOperand (this->parseCondition ());
        this->accept ({TokenType::ParenthClose});
    }
    else
    {
        if (this->peek ({TokenType::Identifier}))
        {
            node->addOperand (this->parseVariable ());
        }
        else
        {
            node->addOperand (this->parseLiteral ());
        }
    }

    if (!node->isNegated ())
    {
        //this->tracer.leave ();
        return node->getLeftSide ();
    }

    //this->tracer.leave ();
    return node;
}