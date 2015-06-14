// /====================================\
// | SIMPLE SCRIPT LANGUAGE INTERPRETER |
// \====================================/
//
// @author Paweł Kaczyński
// --------------------------------------

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include "Interpreter.h"
#include "MessageHandler.h"

int main (int argc, char *argv[])
{
#ifdef _WIN32
    HANDLE winConsole;
    winConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(winConsole, 15);
#endif

    MessageHandler::debug(" ::: Debug mode enabled :::\n");

    Interpreter interpreter (argc, argv);
    if (interpreter.run ())
        MessageHandler::info(" ::: Press Return to exit :::");
    else
        MessageHandler::error(" ::: Press Return to exit :::");
    std::cin.get ();
    return 0;
}
