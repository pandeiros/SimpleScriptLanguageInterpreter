#include "Interpreter.h"

#include "Lexer.h"
#include "Parser.h"
#include "SemanticChecker.h"
#include "Executor.h"
#include "MessageHandler.h"

Interpreter::Interpreter(int argc, char* argv[]) : _argCount(argc)
{
    if (argc > 1)
    {
        // First parameter should be source file.
        std::string strSource(argv[1]);
        _sourceFile = strSource;

        if (argc > 2)
        {
            // Process the rest of parameters for program arguments.
            for (int i = 2; i < argc; ++i)
            {
                std::string strArg(argv[i]);
                _vecArgs.push_back(strArg);
            }
        }
    }
    else
    {
        MessageHandler::error("Input file missing!");
    }
}

bool Interpreter::run()
{
#ifdef _DEBUG
    this->printParams();
#endif

    Lexer lexer(_sourceFile);
    Parser parser(lexer);
    SemanticChecker semCheck;
    Executor executor;

    MessageHandler::debug(" ::: Parsing started :::");
    auto program = parser.parse();

    if (program.get()->_functions.size() > 0 && parser.getParsingSucceeded())
    {
        MessageHandler::debug(" ::: Semantic analysis started :::");

        // TODO Pass 'program' function arguments.
        auto checkResult = semCheck.checkAll(program);

        if (checkResult.size() > 0 && semCheck.getCheckSucceeded())
        {
            MessageHandler::debug(" ::: Execution started :::");
            executor.execute(checkResult);
        }
        else
        {
            MessageHandler::error(" ::: SEMANTIC ANALYSIS FAILED! :::");
            return false;
        }
    }
    else
    {
        MessageHandler::error(" ::: PARSING FAILED! :::");
        return false;
    }

    return true;
}