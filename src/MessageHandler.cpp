#include "MessageHandler.h"

void MessageHandler::error(const std::string& message)
{
    MessageHandler::printLabel(ERROR);
    MessageHandler::print(message);
}

void MessageHandler::warning(WARNING)
{
    MessageHandler::printLabel("Warning", "yellow");
    MessageHandler::print(message);
}

void MessageHandler::info(const std::string& message)
{
    MessageHandler::printLabel(INFO);
    MessageHandler::print(message);
}

void MessageHandler::printLabel(const Type type)
{
    std::string label = "";
    int colorCode = 0;

    switch (type)
    {
        case INFO:
            colorCode = 34;
            label = "INFO";
            break;
        case WARNING:
            colorCode = 33;
            label = "WARNING";
            break;
        case ERROR:
            colorCode = 31;
            label = "ERROR";
            break;
        default:
            colorCode = 36;
            label = "DEFAULT";
            break;

    }

    std::cout << "[\e[" << colorCode << "m" << label << "\e[0m] ";
}

void MessageHandler::print(const std::string & message)
{
    std::cout << message << std::endl;
}
