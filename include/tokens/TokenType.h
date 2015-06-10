#ifndef __TOKEN_TYPE_H__
#define __TOKEN_TYPE_H__

#include <unordered_map>
#include <string>

enum class TokenType
{
    Function,
    MainProgram,
    ParenthOpen,
    ParenthClose,
    BracketOpen,
    BracketClose,
    SquareBracketOpen,
    SquareBracketClose,
    Semicolon,

    Var,
    Assignment,
    Integer,
    String,
    Float,
    Bool,
    Identifier,
    Type,
    Quotation,
    True,
    False,

    Comma,
    If,
    While,
    Else,
    Return,
    Continue,
    Break,
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
    Infinity,
    Dot,
    NumberLiteral,
    Invalid,
    EndOfFile,
    Undefined
};

const std::unordered_map<TokenType, std::string> tokenTypeNames =
{
    {TokenType::MainProgram, "MainProgram"},
    {TokenType::Function, "Function"},
    {TokenType::ParenthOpen, "ParenthOpen"},
    {TokenType::ParenthClose, "ParenthClose"},
    {TokenType::BracketOpen, "BracketOpen"},
    {TokenType::BracketClose, "BracketClose"},
    {TokenType::Semicolon, "Semicolon"},
    {TokenType::Return, "Return"},

    {TokenType::Var, "Var"},
    {TokenType::Assignment, "Assignment"},
    {TokenType::Integer, "Int"},
    {TokenType::Float, "Float"},
    {TokenType::Bool, "Bool"},
    {TokenType::String, "String"},
    {TokenType::Identifier, "Identifier"},      // TODO Change to "Name"
    {TokenType::Comma, "Comma"},
    {TokenType::Quotation, "Quotation"},
    {TokenType::True, "True"},
    {TokenType::False, "False"},
   // {TokenType::Type, "Type"},

    {TokenType::SquareBracketOpen, "SquareBracketOpen"},
    {TokenType::SquareBracketClose, "SquareBracketClose"},
    {TokenType::If, "If"},
    {TokenType::While, "While"},
    {TokenType::Else, "Else"},
    {TokenType::Continue, "Continue"},
    {TokenType::Break, "Break"},
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
    {TokenType::Infinity, "Infinity"},
    {TokenType::Dot, "Dot"},
    {TokenType::NumberLiteral, "NumberLiteral"},

    {TokenType::Invalid, "Invalid"},
    {TokenType::EndOfFile, "EndOfFile"},
    {TokenType::Undefined, "Undefined"}
};

static const std::unordered_map<std::string, TokenType> & keywords =
{
    {"function", TokenType::Function},
    {"program", TokenType::MainProgram},
    {"return", TokenType::Return},

    {"var", TokenType::Var},
    {"int", TokenType::Integer},
    {"bool", TokenType::Bool},
    {"string", TokenType::String },
    {"float", TokenType::Float},
    {"true", TokenType::True},
    {"false", TokenType::False},

    {"if", TokenType::If},
    {"while", TokenType::While},
    {"else", TokenType::Else},
    {"continue", TokenType::Continue},
    {"break", TokenType::Break},
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
    {"\"", TokenType::Quotation}
};


inline const std::string getTokenTypeName(const TokenType & tokenType)
{
    return tokenTypeNames.at(tokenType);
};

#endif // __TOKEN_TYPE_H__
