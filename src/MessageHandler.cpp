#include "MessageHandler.h"

void MessageHandler::error(const std::string & message)
{
    MessageHandler::printLabel(ERROR);
    MessageHandler::print(message);
}

void MessageHandler::warning(const std::string & message)
{
    MessageHandler::printLabel(WARNING);
    MessageHandler::print(message);
}

void MessageHandler::info(const std::string & message)
{
    MessageHandler::printLabel(INFO);
    MessageHandler::print(message);
}

void MessageHandler::debug(const std::string & message)
{
#ifdef _DEBUG
    MessageHandler::printLabel(DEBUG);
    MessageHandler::print(message);
#endif
}

void MessageHandler::token(const std::string & name)
{
#ifdef _DEBUG
    MessageHandler::printLabel(TOKEN);
    MessageHandler::print(name);
#endif
}

void MessageHandler::unexpectedToken(const std::string name, const std::string line,
                                     const std::string pos, const std::string lineOcc,
                                     const std::string marker)
{
    MessageHandler::error(std::string("Unexpected token: ").append(name)
                          .append(" (Line: ").append(line)
                          .append(", Pos: ").append(pos)
                          .append(")").append("\n").append(lineOcc)
                          .append("\n").append(marker));
}

void MessageHandler::printLabel(const Type type)
{
    std::string label = "";
    int colorCode = 0;

    switch (type)
    {
        case INFO:
            colorCode = 34;
            label = " INFO  ";
            break;
        case WARNING:
            colorCode = 33;
            label = "WARNING";
            break;
        case ERROR:
            colorCode = 31;
            label = " ERROR ";
            break;
        case DEBUG:
            colorCode = 35;
            label = " DEBUG ";
            break;
        case TOKEN:
            colorCode = 32;
            label = " TOKEN ";
            break;
        default:
            colorCode = 37;
            label = "DEFAULT";
            break;

    }

    std::cout << "<<\e[" << colorCode << "m" << label << "\e[0m>> ";
}

void MessageHandler::print(const std::string & message)
{
    std::cout << message << std::endl;
}
