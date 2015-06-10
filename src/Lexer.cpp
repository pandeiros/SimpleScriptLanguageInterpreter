#include "Lexer.h"

#include <algorithm>

Lexer::Lexer(const std::string & file)
{
    _inputManager.setSourceFile(file);
}

const Token Lexer::nextToken()
{
    Token token;
    auto character = this->_inputManager.nextCharacter();

    // Ignore white characters.
    while (isspace(character))
    {
        character = this->_inputManager.nextCharacter();
    }

    // Occurence details in the source.
    token._line = this->_inputManager.getCurrentLineNo();
    token._col = this->_inputManager.getCurrentSignPos() - 1;
    token._lineStart = this->_inputManager.getCurrentLinePos();

    // Return EOF token if EOF found.
    if (this->_inputManager.hasFinished())
    {
        token._type = TokenType::EndOfFile;
        return token;
    }

    // Looking for KEYWORD od IDENTIFIER (because it has to begin with letter or '_')
    if (isalpha(character) || character == '_' || character == '$')    // TODO Remove $
    {
        // Character buffer.
        std::string buffer;

        // Read as many characters as possible.
        do
        {
            buffer.push_back(character);
            character = this->_inputManager.nextCharacter();
        }
        while (isalnum(character) || character == '_' || character == '$'); // TODO Remove $

        this->_inputManager.rewind();   // TODO Change

        if (keywords.count(buffer) == 1)
        {
            // KEYWORD found.
            token._type = keywords.at(buffer);
        }
        else
        {
            // IDENTIFIER FOUND
            token._type = TokenType::Identifier;
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
            character = this->_inputManager.nextCharacter();
        }
        while (isdigit(character) || character == '.');

        this->_inputManager.rewind();

        token._type = TokenType::NumberLiteral;
        token._value = buffer;
    }
    // Other characters.
    // TODO Check for string (charactere between " ").
    else
    {
        // Special characters
        switch (character)
        {
            case '=':
            {
                if (this->_inputManager.nextCharacter() == '=')
                {
                    token._type = TokenType::Equality;
                }
                else
                {
                    this->_inputManager.rewind();
                    token._type = TokenType::Assignment;
                }
                break;
            }
            case '<':
            {
                if (this->_inputManager.nextCharacter() == '=')
                {
                    token._type = TokenType::LessOrEqual;
                }
                else
                {
                    this->_inputManager.rewind();
                    token._type = TokenType::Less;
                }
                break;
            }
            case '>':
            {
                if (this->_inputManager.nextCharacter() == '=')
                {
                    token._type = TokenType::GreaterOrEqual;
                }
                else
                {
                    this->_inputManager.rewind();
                    token._type = TokenType::Greater;
                }
                break;
            }
            case '!':
            {
                if (this->_inputManager.nextCharacter() == '=')
                {
                    token._type = TokenType::Inequality;
                }
                else
                {
                    this->_inputManager.rewind();
                    token._type = TokenType::Negation;
                }
                break;
            }
            case '&':
            {
                if (this->_inputManager.nextCharacter() == '&')
                {
                    token._type = TokenType::And;
                }
                else
                {
                    this->_inputManager.rewind();
                    token._type = TokenType::Invalid;
                }
                break;
            }
            case '|':
            {
                if (this->_inputManager.nextCharacter() == '|')
                {
                    token._type = TokenType::Or;
                }
                else
                {
                    this->_inputManager.rewind();
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
    }

    return token;
}

const std::string Lexer::getLine(const std::streampos& linePos)
{
    return this->_inputManager.getLine(linePos);
}