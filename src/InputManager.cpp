#include "InputManager.h"

#include "MessageHandler.h"


//FileReader::FileReader (const std::string& file) :
//handler (file)
//{
//    if (this->handler.fail ())
//    {
//        ErrorHandler::error (std::string ("Failed to open file: ").append (file));
//    }
//
//    this->currentLinePos = this->handler.tellg ();
//}

const char InputManager::nextSign ()
{
    char sign;

    while (true)
    {
        sign = this->_handler.get ();

        if (sign == '\n' || sign == '\r')
        {
            if (this->_previousSign != sign &&
                (this->_previousSign == '\n' || this->_previousSign == '\r')
                )
            {
                // CRLF or LFCR, just skip it
            }
            else
            {
                this->_currentLineNo++;
                this->_currentSignPos = 0;
                this->_currentLinePos = this->_handler.tellg ();
            }

            this->_previousSign = sign;
            continue;
        }

        this->_previousSign = sign;
        this->_currentSignPos++;

        return sign;
    }
}

void InputManager::rewind ()
{
    this->_handler.unget ().unget ();

    this->_previousSign = this->_handler.get ();
    auto peek = this->_handler.peek ();

    if (peek == '\n' || peek == '\r')
    {
        this->_previousSign = this->_handler.get ();
        return;
    }

    this->_currentSignPos--;
}

const bool InputManager::hasFinished () const
{
    return this->_handler.eof ();
}

const unsigned int & InputManager::getCurrentLineNo () const
{
    return this->_currentLineNo;
}

const unsigned int & InputManager::getCurrentSignPos () const
{
    return this->_currentSignPos;
}

const std::streampos InputManager::getCurrentLinePos () const
{
    return this->_currentLinePos;
}

const std::string InputManager::getLine (const std::streampos& linePos)
{
    const std::streampos currentPos = this->_handler.tellg ();
    std::string line;

    this->_handler.seekg (linePos);

    std::getline (this->_handler, line);

    this->_handler.seekg (currentPos);

    return line;
}
