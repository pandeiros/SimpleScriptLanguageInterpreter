// /====================================\
// | SIMPLE SCRIPT LANGUAGE INTERPRETER |
// \====================================/
//
// @author Paweł Kaczyński
// --------------------------------------

#include <iostream>
#include "Interpreter.h"

int main (int argc, char *argv[])
{
    Interpreter interpreter (argc, argv);
    interpreter.run ();

    std::cout << "(Press Return to exit)";
    std::cin.get ();
    return 0;
}
