#ifndef __LEXER_H__
#define __LEXER_H__

#include "Token.h"
#include "InputManager.h"

/*
 * Organizes read symbols from the InputManager into tokens.
 * Uses premade token list.
 */
class Lexer
{
public:
    Lexer (const std::string & file);

    // Returns next token generated from input.
    // Default returned token is of type Invalid, in case of no match.
    const Token nextToken ();

    // Return line where the token occured.
    const std::string getLine (const std::streampos& linePos);

private:
    // Class providing input characters.
    InputManager _inputManager;
};

#endif // __LEXER_H__