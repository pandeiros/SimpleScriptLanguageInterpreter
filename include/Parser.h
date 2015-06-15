#ifndef __PARSER_H__
#define __PARSER_H__

#include "Token.h"
#include "Syntax.h"

#define PRINT_UNEXP (MessageHandler::unexpectedToken(getTokenTypeName(token._type), \
                    std::to_string(token._line), \
                    std::to_string(token._col), \
                    _lexer.getLine(token._lineStart), \
                    getErrorIndicator(token._col, _lexer.getLine(token._lineStart))) )
#define FAIL (_processSucceeded = false)

#ifdef EXIT_ON_FAIL
#define CHECK_FAIL(_RET_) \
    if (!_processSucceeded) \
        return _RET_
#else
#define CHECK_FAIL(_RET_)
#endif

class Lexer;

/*
 * Checks, whether tokens are provided accordingly to language grammar.
 */
class Parser
{
public:
    Parser(Lexer & lexer) : _lexer(lexer)
    {};

    // Main parsing method. Initiate subsequent decompositions.
    std::shared_ptr<syntax::Program> parse();

    // Success flag getter.
    inline bool getParsingSucceeded()
    {
        return _processSucceeded;
    }

private:
    /* Parser tools for managing received tokens. */

    // Check, whether token received is acceptable (is on the acceptable list).
    bool isAcceptable(const Token & token, const std::initializer_list<TokenType> & acceptableTokens) const;
    Token accept(const std::initializer_list<TokenType> & acceptableTokens);

    // Token peeking.
    bool peek(const std::initializer_list<TokenType> & acceptableTokens);
    Token getPeeked();
    void peekFail();

    // Return "^" character to indicate place where the error occurs.
    const std::string getErrorIndicator(const unsigned int & pos, const std::string & line);

    // Some buffer managing methods.
    bool hasBufferedToken() const;
    void resetPreviousToken();
    void typeFail(const std::string & type);

    /* Decomposition procedures */

    // Functions.
    std::shared_ptr<syntax::FunctionDefinition> parseFunction();
    bool parseParameters(std::vector<std::string> & types, std::vector<std::string> & names);
    std::shared_ptr<syntax::StatementBlock> parseStatementBlock();
    std::shared_ptr<syntax::StatementBlock> parseSingleInstruction();

    // Basic instructions.
    std::shared_ptr<syntax::ReturnStatement> parseReturnStatement();
    std::shared_ptr<syntax::VarDeclaration> parseVarDeclaration();
    std::shared_ptr<syntax::ConstDeclaration> parseConstDeclaration();
    std::shared_ptr<syntax::RValue> parseAssignable(std::string type = "");
    std::shared_ptr<syntax::Call> parseFunctionCall(const std::string & identifier);

    // Expressions
    std::shared_ptr<syntax::RValue> parseExpression(const Token & initToken = Token(TokenType::Undefined));
    std::shared_ptr<syntax::RValue> parseLogicalExpression(const Token & initToken = Token(TokenType::Undefined));
    std::shared_ptr<syntax::RValue> parseArithmeticExpression(const Token & initToken = Token(TokenType::Undefined));

    // Arithmetic expression.
    std::shared_ptr<syntax::ArithmeticExpression> parseStrongArithmeticExpression(const Token & initToken = Token(TokenType::Undefined));
    NodePtr parseArithmeticOperand(const Token & initToken = Token(TokenType::Undefined));

    // Logical expression.
    std::shared_ptr<syntax::LogicalExpression> parseStrongLogicalExpression(const Token & initToken = Token(TokenType::Undefined));
    std::shared_ptr<syntax::LogicalExpression> parseRelationExpression(const Token & initToken = Token(TokenType::Undefined));
    NodePtr parseLogicalOperand(const Token & initToken = Token(TokenType::Undefined));

    // Complex instructions.
    std::shared_ptr<syntax::IfStatement> parseIfStatement();
    std::shared_ptr<syntax::WhileStatement> parseWhileStatement();

    // Variables and literals
    std::shared_ptr<syntax::Variable> parseVariable(const Token& firstToken = Token(TokenType::Undefined));
    NodePtr parseLiteral();
    std::shared_ptr<syntax::Literal> parseString(std::string type = "");
    std::shared_ptr<syntax::Literal> parseBool(std::string type = "");
    std::shared_ptr<syntax::Literal> parseNumber(std::string type = "");
    NodePtr parseAssignmentOrFunctionCall();
    NodePtr parseVariableOrFunctionCall();

    // Lexer for receiving tokens.
    Lexer & _lexer;

    // Saved previous token.
    Token _previousToken;

    // Success flag.
    bool _processSucceeded = true;
};

#endif // __PARSER_H__