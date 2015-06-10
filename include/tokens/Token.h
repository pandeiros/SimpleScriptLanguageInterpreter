#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "TokenType.h"
#include <string>

/*
 * Lexer generated tokens. Fall under certain type.
 * Remembers also line and position of occurence in the source.
 */
class Token
{
public:
    Token () = default;

    Token (const TokenType & type) : _type (type)
    {}

    // Default options in case of no match.
    TokenType _type = TokenType::Invalid;
    std::string _value = "";

    // Occurence details.
    unsigned int _line = 0;
    unsigned int _col = 0;
    std::streampos _lineStart;
};

#endif // __TOKEN_H__
