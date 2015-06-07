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

    std::shared_ptr<Program> parse ();

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
    std::shared_ptr<FunDefinition> parseFunction ();
    std::vector<std::string> parseParameters ();
    std::shared_ptr<StatementBlock> parseStatementBlock ();

    std::shared_ptr<IfStatement> parseIfStatement ();
    std::shared_ptr<WhileStatement> parseWhileStatement ();
    std::shared_ptr<ReturnStatement> parseReturnStatement ();
    std::shared_ptr<VarDeclaration> parseInitStatement ();
    NodePtr parseAssignmentOrFunCall ();
    std::shared_ptr<LoopJump> parseLoopJump ();

    std::shared_ptr<Assignable> parseAssignable ();
    std::shared_ptr<Call> parseFunCall (const std::string& identifier);
    std::shared_ptr<Variable> parseVariable (const Token& firstToken = Token (TokenType::Undefined));
    std::shared_ptr<Matrix> parseLiteral ();
    double parseNumberLiteral ();
    std::shared_ptr<Matrix> parseMatrixLiteral ();

    std::shared_ptr<Expression> parseExpression (const Token& firstToken = Token (TokenType::Undefined));
    std::shared_ptr<Expression> parseMultiplicativeExpression (const Token& firstToken = Token (TokenType::Undefined));
    NodePtr parsePrimaryExpression (const Token& firstToken = Token (TokenType::Undefined));

    std::shared_ptr<Condition> parseCondition ();
    std::shared_ptr<Condition> parseAndCondition ();
    std::shared_ptr<Condition> parseEqualityCondition ();
    std::shared_ptr<Condition> parseRelationalCondition ();
    NodePtr parsePrimaryCondition ();
};

#endif // __PARSER_H__