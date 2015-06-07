#ifndef __LEXER_H__
#define __LEXER_H__

#include "Token.h"
#include "InputManager.h"

/*
 *
 */
class Lexer
{
public:
    Lexer (const std::string & file);

    const Token nextToken ();
    const std::string getLine (const std::streampos& linePos);

private:
    InputManager _inputManager;
};

#endif // __LEXER_H__