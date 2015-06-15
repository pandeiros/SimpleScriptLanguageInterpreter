#include "Lexer.h"

#include <algorithm>

Lexer::Lexer(const std::string & file)
{
    _inputManager.setSourceFile(file);
}

const Token Lexer::nextToken()
{
    Token token;
    auto character = _inputManager.nextCharacter();

    // Ignore white characters.
    while (isspace(character))
    {
        character = _inputManager.nextCharacter();
    }

    // Occurence details in the source.
    token._line = _inputManager.getCurrentLineNo();
    token._col = _inputManager.getCurrentSignPos() - 1;
    token._lineStart = _inputManager.getCurrentLinePos();

    // Return EOF token if EOF found.
    if (_inputManager.hasFinished())
    {
        token._type = TokenType::EndOfFile;
        return token;
    }

    // Looking for KEYWORD od IDENTIFIER (because it has to begin with letter or '_')
    if (isalpha(character) || character == '_')
    {
        // Character buffer.
        std::string buffer;

        // Read as many characters as possible.
        do
        {
            buffer.push_back(character);
            character = _inputManager.nextCharacter();
        }
        while (isalnum(character) || character == '_');

        _inputManager.rewind();

        if (keywords.count(buffer) == 1)
        {
            // KEYWORD found.
            token._type = keywords.at(buffer);
            token._value = buffer;
        }
        else
        {
            // IDENTIFIER FOUND
            token._type = TokenType::Name;
            token._value = buffer;
        }
    }

    // Looking for NUMBER
    else if (isdigit(character))
    {
        // Read as many characters as possible.
        std::string buffer;
        do
        {
            buffer.push_back(character);
            character = _inputManager.nextCharacter();
        }
        while (isdigit(character) || character == '.');

        _inputManager.rewind();

        token._type = TokenType::NumberLiteral;
        token._value = buffer;
    }

    // Parse STRING
    else if (character == '"')
    {
        // Character buffer.
        std::string buffer;
        character = _inputManager.nextCharacter();

        // Read as many characters as possible.
        while (character != '"')
        {
            buffer.push_back(character);
            character = _inputManager.nextCharacter();
        }

        token._type = TokenType::StringLiteral;
        token._value = buffer;
    }

    // Parse COMMENT
    else if (character == '/')
    {
        // Check for Division character.
        if (_inputManager.nextCharacter() != '/')
        {
            _inputManager.rewind();
            token._type = TokenType::Divide;
            token._value = tokenTypeNames.at(TokenType::Divide);
        }
        else
        {
            // Ignore all character in the line.
            while (_inputManager.nextCharacter(true) != '\n');
            token._type = TokenType::Comment;
            token._value = tokenTypeNames.at(TokenType::Comment);
        }
    }
    // Other characters.
    else
    {
        // Special characters
        switch (character)
        {
            case '=':
            {
                if (_inputManager.nextCharacter() == '=')
                {
                    token._type = TokenType::Equality;
                }
                else
                {
                    _inputManager.rewind();
                    token._type = TokenType::Assignment;
                }
                break;
            }
            case '<':
            {
                if (_inputManager.nextCharacter() == '=')
                {
                    token._type = TokenType::LessOrEqual;
                }
                else
                {
                    _inputManager.rewind();
                    token._type = TokenType::Less;
                }
                break;
            }
            case '>':
            {
                if (_inputManager.nextCharacter() == '=')
                {
                    token._type = TokenType::GreaterOrEqual;
                }
                else
                {
                    _inputManager.rewind();
                    token._type = TokenType::Greater;
                }
                break;
            }
            case '!':
            {
                if (_inputManager.nextCharacter() == '=')
                {
                    token._type = TokenType::Inequality;
                }
                else
                {
                    _inputManager.rewind();
                    token._type = TokenType::Negation;
                }
                break;
            }
            case '&':
            {
                if (_inputManager.nextCharacter() == '&')
                {
                    token._type = TokenType::And;
                }
                else
                {
                    _inputManager.rewind();
                    token._type = TokenType::Invalid;
                }
                break;
            }
            case '|':
            {
                if (_inputManager.nextCharacter() == '|')
                {
                    token._type = TokenType::Or;
                }
                else
                {
                    _inputManager.rewind();
                    token._type = TokenType::Invalid;
                }
                break;
            }
            default:
            {
                if (simpleSigns.count(character) == 1)
                {
                    token._type = simpleSigns.at(character);
                }
                else
                {
                    token._type = TokenType::Invalid;
                }
                break;
            }
        }

        token._value = tokenTypeNames.at(token._type);
    }

    return token;
}

const std::string Lexer::getLine(const std::streampos& linePos)
{
    return _inputManager.getLine(linePos);
}