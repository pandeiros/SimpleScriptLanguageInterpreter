#ifndef __INTERPRETER_H__
#define __INTERPRETER_H__

#include <string>
#include <vector>
#include <iostream>

/**
 * Main program class. Controls flow of the interpreter itself.
 * Also acquire parameters of the 'main' function.
 */
class Interpreter
{
public:
    Interpreter(int argc, char * argv[]);

    // Main program control function.
    bool run();

private:
    // Parameters control members.
    unsigned int _argCount;
    std::string _sourceFile;
    std::vector<std::string> _vecArgs;


#ifdef _DEBUG
    void printParams()
    {
        std::cout << "      --- INPUT ---\n" << "==========================\n";
        std::cout << "Source file: " << _sourceFile << "\n";
        std::cout << "Arguments:   ";
        for (unsigned int i = 0; i < _vecArgs.size(); ++i)
            std::cout << _vecArgs[i] << "  ";
        std::cout << "\n";
        std::cout << "==========================\n\n";
    }
#endif

};

#endif // __INTERPRETER_H__
