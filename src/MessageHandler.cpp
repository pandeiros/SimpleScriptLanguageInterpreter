#include "MessageHandler.h"

#ifdef _WIN32
#include <windows.h>
#endif

void MessageHandler::error(const std::string & message)
{
    MessageHandler::printLabel(_ERROR_);
    MessageHandler::print(message);
}

void MessageHandler::warning(const std::string & message)
{
    MessageHandler::printLabel(_WARNING_);
    MessageHandler::print(message);
}

void MessageHandler::info(const std::string & message)
{
    MessageHandler::printLabel(_INFO_);
    MessageHandler::print(message);
}

void MessageHandler::debug(const std::string & message)
{
#ifdef _DEBUG
    MessageHandler::printLabel(_DEBUG_);
    MessageHandler::print(message);
#endif
}

void MessageHandler::token(const std::string & name)
{
#ifdef _DEBUG
    MessageHandler::printLabel(_TOKEN_);
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
    int colorCode = getColorAndLabel(type, label);

#ifndef _WIN32
    std::cout << "<<\e[" << colorCode << "m" << label << "\e[0m>> ";
#else
    HANDLE winConsole;
    winConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(winConsole, colorCode);
    std::cout << "<<" << label << ">> ";
    SetConsoleTextAttribute(winConsole, 15);
#endif
}

void MessageHandler::print(const std::string & message)
{
    std::cout << message << std::endl;
}

int MessageHandler::getColorAndLabel(const Type type, std::string & label)
{
    int colorCode = -1;

    switch (type)
    {
#ifdef _WIN32
        case _INFO_:
            colorCode = 10;
            label = " INFO  ";
            break;
        case _WARNING_:
            colorCode = 14;
            label = "WARNING";
            break;
        case _ERROR_:
            colorCode = 12;
            label = " ERROR ";
            break;
        case _DEBUG_:
            colorCode = 11;
            label = " DEBUG ";
            break;
        case _TOKEN_:
            colorCode = 8;
            label = " TOKEN ";
            break;
        default:
            colorCode = 8;
            label = "DEFAULT";
            break;
#else
        case _INFO_:
            colorCode = 34;
            label = " INFO  ";
            break;
        case _WARNING_:
            colorCode = 33;
            label = "WARNING";
            break;
        case _ERROR_:
            colorCode = 31;
            label = " ERROR ";
            break;
        case _DEBUG_:
            colorCode = 35;
            label = " DEBUG ";
            break;
        case _TOKEN_:
            colorCode = 32;
            label = " TOKEN ";
            break;
        default:
            colorCode = 0;
            label = "DEFAULT";
            break;
#endif
    }

    return colorCode;
}
