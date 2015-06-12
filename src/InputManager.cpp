#include "InputManager.h"
#include "MessageHandler.h"

const char InputManager::nextCharacter(bool isComment)
{
    while (true)
    {
        _previousCharacter = _character;
        _character = _handler.get();

        // New line.
        if (_character == '\n' || _character == '\r')
        {
            // Ex. We got \n but wait for \r to increment line count (or other way around).
            if (_previousCharacter != _character &&
                (_previousCharacter == '\n' || _previousCharacter == '\r'))
            {
                // DO NOTHING
            }
            else
            {
                _currentLineNo++;
                _currentSignPos = 0;
                _currentLinePos = _handler.tellg();

                if (isComment)
                    return '\n';
            }

            // Reading empty lines.
            continue;
        }

        _currentSignPos++;
        return _character;
    }
}

// TODO Use buffer instead.
void InputManager::rewind()
{
    _handler.unget().unget();

    _previousCharacter = _handler.get();
    auto peek = _handler.peek();

    if (peek == '\n' || peek == '\r')
    {
        _previousCharacter = _handler.get();
        return;
    }

    _currentSignPos--;
}

const bool InputManager::hasFinished() const
{
    return _handler.eof();
}

const unsigned int & InputManager::getCurrentLineNo() const
{
    return _currentLineNo;
}

const unsigned int & InputManager::getCurrentSignPos() const
{
    return _currentSignPos;
}

const std::streampos InputManager::getCurrentLinePos() const
{
    return _currentLinePos;
}

const std::string InputManager::getLine(const std::streampos& linePos)
{
    const std::streampos currentPos = _handler.tellg();
    std::string line;

    _handler.seekg(linePos);
    std::getline(_handler, line);
    _handler.seekg(currentPos);

    return line;
}
