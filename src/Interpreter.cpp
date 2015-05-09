#include "Interpreter.h"

Interpreter::Interpreter (int argc, char* argv[]) : _argCount (argc)
{
    if (argc > 1)
    {
        // First parameter should be source file.
        std::string strSource (argv[1]);
        this->_sourceFile = strSource;

        if (argc > 2)
        {
            // Process the rest of parameters for program arguments.
            for (int i = 2; i < argc; ++i)
            {
                std::string strArg (argv[i]);
                _vecArgs.push_back (strArg);
            }
        }
    }
}

void Interpreter::run ()
{
#ifdef DEBUG
    this->printParams ();
#endif

    _inputManager.setSourceFile (_sourceFile);
}