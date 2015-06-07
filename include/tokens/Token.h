#ifndef __TOKEN_H__
#define __TOKEN_H__

#include "TokenType.h"
#include <string>

/*
 *
 */
class Token
{
public:
    Token () = default;

    Token (const TokenType & type) : _type (type)
    {}

    TokenType _type = TokenType::Invalid;
    std::string _value = "";

    unsigned int _line = 0;
    unsigned int _col = 0;
    std::streampos _lineStart;
};

#endif // __TOKEN_H__
