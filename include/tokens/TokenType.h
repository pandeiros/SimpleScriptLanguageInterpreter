#ifndef __TOKEN_TYPE_H__
#define __TOKEN_TYPE_H__

#include <unordered_map>
#include <string>

enum class TokenType
{
    Program,
    Function,
    ParenthOpen,
    ParenthClose,
    BracketOpen,
    BracketClose,
    SquareBracketOpen,
    SquareBracketClose,
    Semicolon,
    Return,

    Var,
    Const,
    Assignment,
    Integer,
    String,
    Float,
    Bool,
    Name,
    Type,
    Quotation,
    True,
    False,
    StringLiteral,
    NumberLiteral,

    Comma,
    If,
    While,
    Else,
    Negation,
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
    Dot,

    Comment,
    Invalid,
    EndOfFile,
    Undefined
};

const std::unordered_map<TokenType, std::string> tokenTypeNames =
{
    {TokenType::Program, "MainProgram"},
    {TokenType::Function, "Function"},
    {TokenType::ParenthOpen, "ParenthOpen"},
    {TokenType::ParenthClose, "ParenthClose"},
    {TokenType::BracketOpen, "BracketOpen"},
    {TokenType::BracketClose, "BracketClose"},
    {TokenType::Semicolon, "Semicolon"},
    {TokenType::Return, "Return"},

    {TokenType::Var, "Var"},
    {TokenType::Var, "Const"},
    {TokenType::Assignment, "Assignment"},
    {TokenType::Integer, "Int"},
    {TokenType::Float, "Float"},
    {TokenType::Bool, "Bool"},
    {TokenType::String, "String"},
    {TokenType::Name, "Name"},
    {TokenType::Comma, "Comma"},
    {TokenType::Quotation, "Quotation"},    // TODO Check if used 
    {TokenType::True, "True"},
    {TokenType::False, "False"},
    {TokenType::StringLiteral, "StringLiteral"},

    {TokenType::SquareBracketOpen, "SquareBracketOpen"},
    {TokenType::SquareBracketClose, "SquareBracketClose"},
    {TokenType::If, "If"},
    {TokenType::While, "While"},
    {TokenType::Else, "Else"},
    {TokenType::Negation, "Negation"},
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
    {TokenType::Dot, "Dot"},
    {TokenType::NumberLiteral, "NumberLiteral"},

    {TokenType::Comment, "Comment"},
    {TokenType::Invalid, "Invalid"},
    {TokenType::EndOfFile, "EndOfFile"},
    {TokenType::Undefined, "Undefined"}
};

static const std::unordered_map<std::string, TokenType> & keywords =
{
    {"function", TokenType::Function},
    {"program", TokenType::Program},
    {"return", TokenType::Return},

    {"var", TokenType::Var},
    {"const", TokenType::Const},
    {"int", TokenType::Integer},
    {"bool", TokenType::Bool},
    {"string", TokenType::String},
    {"float", TokenType::Float},
    {"true", TokenType::True},
    {"false", TokenType::False},

    {"if", TokenType::If},
    {"while", TokenType::While},
    {"else", TokenType::Else},
};

static const std::unordered_map<char, TokenType> & simpleSigns =
{
    {'(', TokenType::ParenthOpen},
    {')', TokenType::ParenthClose},
    {'{', TokenType::BracketOpen},
    {'}', TokenType::BracketClose},
    {';', TokenType::Semicolon},

    {'[', TokenType::SquareBracketOpen},
    {']', TokenType::SquareBracketClose},
    {',', TokenType::Comma},
    {'+', TokenType::Plus},
    {'-', TokenType::Minus},
    {'*', TokenType::Multiply},
    {'/', TokenType::Divide},
    {'%', TokenType::Modulo},
    {'.', TokenType::Dot},
    {'"', TokenType::Quotation} // TODO Check if used 
};


inline const std::string getTokenTypeName(const TokenType & tokenType)
{
    return tokenTypeNames.at(tokenType);
};

#endif // __TOKEN_TYPE_H__
