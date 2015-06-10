#ifndef __TOKEN_TYPE_H__
#define __TOKEN_TYPE_H__

#include <unordered_map>
#include <string>

enum class TokenType
{
    Function,
    ParenthOpen,
    ParenthClose,
    BracketOpen,
    BracketClose,
    SquareBracketOpen,
    SquareBracketClose,
    Comma,
    Semicolon,
    If,
    While,
    Else,
    Return,
    Continue,
    Break,
    Var,
    Negation,
    Assignment,
    Or,
    And,
    Equality,
    Inequality,
    Less,
    Greater,
    LessOrEqual,
    GreaterOrEqual,
    Plus,
    Minus,
    Multiply,
    Divide,
    Modulo,
    Infinity,
    Dot,
    Identifier,
    NumberLiteral,
    Invalid,
    EndOfFile,
    Undefined
};

const std::unordered_map<TokenType, std::string> tokenTypeNames =
{
    {TokenType::Function, "Function"},
    {TokenType::ParenthOpen, "ParenthOpen"},
    {TokenType::ParenthClose, "ParenthClose"},
    {TokenType::BracketOpen, "BracketOpen"},
    {TokenType::BracketClose, "BracketClose"},
    {TokenType::SquareBracketOpen, "SquareBracketOpen"},
    {TokenType::SquareBracketClose, "SquareBracketClose"},
    {TokenType::Comma, "Comma"},
    {TokenType::Semicolon, "Semicolon"},
    {TokenType::If, "If"},
    {TokenType::While, "While"},
    {TokenType::Else, "Else"},
    {TokenType::Return, "Return"},
    {TokenType::Continue, "Continue"},
    {TokenType::Break, "Break"},
    {TokenType::Var, "Var"},
    {TokenType::Negation, "Negation"},
    {TokenType::Assignment, "Assignment"},
    {TokenType::Or, "Or"},
    {TokenType::And, "And"},
    {TokenType::Equality, "Equality"},
    {TokenType::Inequality, "Inequality"},
    {TokenType::Less, "Less"},
    {TokenType::Greater, "Greater"},
    {TokenType::LessOrEqual, "LessOrEqual"},
    {TokenType::GreaterOrEqual, "GreaterOrEqual"},
    {TokenType::Plus, "Plus"},
    {TokenType::Minus, "Minus"},
    {TokenType::Multiply, "Multiply"},
    {TokenType::Divide, "Divide"},
    {TokenType::Modulo, "Modulo"},
    {TokenType::Infinity, "Infinity"},
    {TokenType::Dot, "Dot"},
    {TokenType::Identifier, "Identifier"},
    {TokenType::NumberLiteral, "NumberLiteral"},
    {TokenType::Invalid, "Invalid"},
    {TokenType::EndOfFile, "EndOfFile"},
    {TokenType::Undefined, "Undefined"}
};

static const std::unordered_map<std::string, TokenType> & keywords =
{
    {"function", TokenType::Function},
    {"if", TokenType::If},
    {"while", TokenType::While},
    {"else", TokenType::Else},
    {"return", TokenType::Return},
    {"continue", TokenType::Continue},
    {"break", TokenType::Break},
    {"var", TokenType::Var},
    {"or", TokenType::Or},
    {"and", TokenType::And},
    {"infinity", TokenType::Infinity}
};

static const std::unordered_map<char, TokenType> & simpleSigns =
{
    {'(', TokenType::ParenthOpen},
    {')', TokenType::ParenthClose},
    {'{', TokenType::BracketOpen},
    {'}', TokenType::BracketClose},
    {'[', TokenType::SquareBracketOpen},
    {']', TokenType::SquareBracketClose},
    {',', TokenType::Comma},
    {';', TokenType::Semicolon},
    {'+', TokenType::Plus},
    {'-', TokenType::Minus},
    {'*', TokenType::Multiply},
    {'/', TokenType::Divide},
    {'%', TokenType::Modulo},
    {'.', TokenType::Dot}
};


inline const std::string getTokenTypeName(const TokenType & tokenType)
{
    return tokenTypeNames.at(tokenType);
};

#endif // __TOKEN_TYPE_H__
