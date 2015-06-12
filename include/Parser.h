#ifndef __PARSER_H__
#define __PARSER_H__

#include "Token.h"
#include "Includes.h"

#define PRINT_UNEXP (MessageHandler::unexpectedToken(getTokenTypeName(token._type), \
                    std::to_string(token._line), \
                    std::to_string(token._col), \
                    _lexer.getLine(token._lineStart), \
                    std::string(token._col, ' ').append("^")) )
#define FAIL (this->_parsingSucceeded = false)

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
        return _parsingSucceeded;
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
    const std::string getErrorIndicator(const unsigned int & pos);

    // Some buffer managing methods.
    bool hasBufferedToken() const;
    void resetPreviousToken();

    /* Decomposition procedures */

    std::shared_ptr<syntax::FunctionDefinition> parseFunction();    //
    bool parseParameters(std::vector<std::string> & types, std::vector<std::string> & names);       //
    std::shared_ptr<syntax::StatementBlock> parseStatementBlock();  //

    std::shared_ptr<syntax::ReturnStatement> parseReturnStatement();    //
    std::shared_ptr<syntax::VarDeclaration> parseVarDeclaration();      //
    std::shared_ptr<syntax::ConstDeclaration> parseConstDeclaration();  //
    std::shared_ptr<syntax::Assignable> parseAssignable(); //
    std::shared_ptr<syntax::Call> parseFunCall(const std::string& identifier); //

    std::shared_ptr<syntax::IfStatement> parseIfStatement();
    std::shared_ptr<syntax::WhileStatement> parseWhileStatement();

    NodePtr parseAssignmentOrFunCall();

    

    std::shared_ptr<syntax::Variable> parseVariable(const Token& firstToken = Token(TokenType::Undefined));
    NodePtr parseLiteral();         //
    std::shared_ptr<syntax::Literal> parseString();     //
    std::shared_ptr<syntax::Literal> parseBool();       //
    std::shared_ptr<syntax::Literal> parseNumber();     //

    std::shared_ptr<syntax::ArithmeticExpression> parseExpression(const Token& firstToken = Token(TokenType::Undefined));
    std::shared_ptr<syntax::ArithmeticExpression> parseMultiplicativeExpression(const Token& firstToken = Token(TokenType::Undefined));
    NodePtr parsePrimaryExpression(const Token& firstToken = Token(TokenType::Undefined));

    std::shared_ptr<syntax::LogicalExpression> parseCondition();
    std::shared_ptr<syntax::LogicalExpression> parseAndCondition();
    std::shared_ptr<syntax::LogicalExpression> parseEqualityCondition();
    std::shared_ptr<syntax::LogicalExpression> parseRelationalCondition();
    NodePtr parsePrimaryCondition();

    // Lexer for receiving tokens.
    Lexer & _lexer;

    // Saved previous token.
    Token _previousToken;

    // Success flag.
    bool _parsingSucceeded = true;
};

#endif // __PARSER_H__