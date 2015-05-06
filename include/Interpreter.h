#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <string>
#include <vector>
#include <iostream>
#include "InputManager.h"

/**
 *
 */
class Interpreter
{
public:

    Interpreter (int argc, char * argv[]);

    void run ();

private:
    unsigned int _argCount;
    std::string _sourceFile;
    std::vector<std::string> _vecArgs;

    InputManager _inputManager;

#ifdef DEBUG
    void printParams ()
    {
        std::cout << "      --- INPUT ---\n" << "==========================\n";
        std::cout << "Source file: " << _sourceFile << "\n";
        std::cout << "Arguments:   ";
        for (unsigned int i = 0; i < _vecArgs.size(); ++i)
            std::cout << _vecArgs[i] << "  ";
        std::cout << "\n";
        std::cout << "==========================\n";
    }
#endif

};

#endif // __INTERPRETER_H__
