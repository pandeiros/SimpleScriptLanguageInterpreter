// /====================================\
// | SIMPLE SCRIPT LANGUAGE INTERPRETER |
// \====================================/
//
// @author Paweł Kaczyński
// --------------------------------------

#include <iostream>
#include "Interpreter.h"
#include "MessageHandler.h"

int main (int argc, char *argv[])
{
#ifdef _DEBUG
    std::cout << "DEBUG MODE ENABLED\n\n";
#endif
    Interpreter interpreter (argc, argv);
    interpreter.run ();

    MessageHandler::info(" ::: Press Return to exit :::");
    std::cin.get ();
    return 0;
}
