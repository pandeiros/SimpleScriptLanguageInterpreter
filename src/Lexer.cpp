#include "Lexer.h"

#include <algorithm>

Lexer::Lexer (const std::string & file)
{
    _inputManager.setSourceFile (file);
}

const Token Lexer::nextToken ()
{
    Token token;
    auto sign = this->_inputManager.nextSign ();

    while (isspace (sign))
    {
        sign = this->_inputManager.nextSign ();
    }

    token._line = this->_inputManager.getCurrentLineNo ();
    token._col = this->_inputManager.getCurrentSignPos () - 1;
    token._lineStart = this->_inputManager.getCurrentLinePos ();

    if (this->_inputManager.hasFinished ())
    {
        token._type = TokenType::EndOfFile;
        return token;
    }

    if (isalpha (sign) || sign == '_' || sign == '$')
    {
        // Keyword or identifier
        std::string buffer;

        do
        {
            buffer.push_back (sign);

            sign = this->_inputManager.nextSign ();
        }
        while (isalnum (sign) || sign == '_' || sign == '$');

        this->_inputManager.rewind ();
        auto tempBuff = buffer;
        std::transform (tempBuff.begin (), tempBuff.end (), tempBuff.begin (), ::tolower);

        if (keywords.count (tempBuff) == 1)
        {
            // Keyword
            token._type = keywords.at (tempBuff);
        }
        else
        {
            // Identifier
            token._type = TokenType::Identifier;
            token._value = buffer;
        }
    }
    else if (isdigit (sign))
    {
        // Finite number
        std::string buffer;

        do
        {
            buffer.push_back (sign);

            sign = this->_inputManager.nextSign ();
        }
        while (isdigit (sign) || sign == '.');

        this->_inputManager.rewind ();

        token._type = TokenType::NumberLiteral;
        token._value = buffer;
    }
    else
    {
        // Special characters

        switch (sign)
        {
            case '=':
            {
                if (this->_inputManager.nextSign () == '=')
                {
                    token._type = TokenType::Equality;
                }
                else
                {
                    this->_inputManager.rewind ();
                    token._type = TokenType::Assignment;
                }
                break;
            }
            case '<':
            {
                if (this->_inputManager.nextSign () == '=')
                {
                    token._type = TokenType::LessOrEqual;
                }
                else
                {
                    this->_inputManager.rewind ();
                    token._type = TokenType::Less;
                }
                break;
            }
            case '>':
            {
                if (this->_inputManager.nextSign () == '=')
                {
                    token._type = TokenType::GreaterOrEqual;
                }
                else
                {
                    this->_inputManager.rewind ();
                    token._type = TokenType::Greater;
                }
                break;
            }
            case '!':
            {
                if (this->_inputManager.nextSign () == '=')
                {
                    token._type = TokenType::Inequality;
                }
                else
                {
                    this->_inputManager.rewind ();
                    token._type = TokenType::Negation;
                }
                break;
            }
            case '&':
            {
                if (this->_inputManager.nextSign () == '&')
                {
                    token._type = TokenType::And;
                }
                else
                {
                    this->_inputManager.rewind ();
                    token._type = TokenType::Invalid;
                }
                break;
            }
            case '|':
            {
                if (this->_inputManager.nextSign () == '|')
                {
                    token._type = TokenType::Or;
                }
                else
                {
                    this->_inputManager.rewind ();
                    token._type = TokenType::Invalid;
                }
                break;
            }
            default:
            {
                if (simpleSigns.count (sign) == 1)
                {
                    token._type = simpleSigns.at (sign);
                }
                else
                {
                    token._type = TokenType::Invalid;
                }
                break;
            }
        }
    }

    return token;
}

const std::string Lexer::getLine (const std::streampos& linePos)
{
    return this->_inputManager.getLine (linePos);
}