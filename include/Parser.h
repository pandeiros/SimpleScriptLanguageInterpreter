#ifndef __PARSER_H__
#define __PARSER_H__

#include "Token.h"
#include "Node.h"

class Lexer;

/*
 *
 */
class Parser
{
public:
    Parser (Lexer& lexer);

    std::shared_ptr<syntax::Program> parse ();

private:
    Lexer & lexer;
    Token previousToken;

    bool isAcceptable (const Token& token, const std::initializer_list<TokenType>& acceptable) const;
    Token accept (const std::initializer_list<TokenType>& acceptable);
    bool peek (const std::initializer_list<TokenType>& acceptable);
    Token getPeeked ();
    void peekFail ();
    const std::string makeErrorMarker (const unsigned int& pos);

    bool hasBufferedToken () const;
    void resetPreviousToken ();

    // Decomposition procedures
    std::shared_ptr<syntax::FunDefinition> parseFunction();
    std::vector<std::string> parseParameters ();
    std::shared_ptr<syntax::StatementBlock> parseStatementBlock();

    std::shared_ptr<syntax::IfStatement> parseIfStatement ();
    std::shared_ptr<syntax::WhileStatement> parseWhileStatement();
    std::shared_ptr<syntax::ReturnStatement> parseReturnStatement();
    std::shared_ptr<syntax::VarDeclaration> parseInitStatement();
    NodePtr parseAssignmentOrFunCall ();
    std::shared_ptr<syntax::LoopJump> parseLoopJump();

    std::shared_ptr<syntax::Assignable> parseAssignable();
    std::shared_ptr<syntax::Call> parseFunCall(const std::string& identifier);
    std::shared_ptr<syntax::Variable> parseVariable(const Token& firstToken = Token(TokenType::Undefined));
    std::shared_ptr<syntax::Matrix> parseLiteral();
    double parseNumberLiteral ();
    std::shared_ptr<syntax::Matrix> parseMatrixLiteral();

    std::shared_ptr<syntax::Expression> parseExpression(const Token& firstToken = Token(TokenType::Undefined));
    std::shared_ptr<syntax::Expression> parseMultiplicativeExpression(const Token& firstToken = Token(TokenType::Undefined));
    NodePtr parsePrimaryExpression (const Token& firstToken = Token (TokenType::Undefined));

    std::shared_ptr<syntax::Condition> parseCondition();
    std::shared_ptr<syntax::Condition> parseAndCondition();
    std::shared_ptr<syntax::Condition> parseEqualityCondition();
    std::shared_ptr<syntax::Condition> parseRelationalCondition();
    NodePtr parsePrimaryCondition ();
};

#endif // __PARSER_H__