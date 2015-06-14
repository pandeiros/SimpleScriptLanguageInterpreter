#ifndef __MESSAGE_HANDLER__
#define __MESSAGE_HANDLER__

#include <string>
#include <iostream>

/**
 * Displays erros and other information in the console.
 */
class MessageHandler
{
public:
    static void error(const std::string & message);
    static void warning(const std::string & message);
    static void info(const std::string & message);
    static void debug(const std::string & message);
    static void token(const std::string & name);
    static void unexpectedToken(const std::string name, const std::string line,
                                const std::string pos, const std::string lineOcc,
                                const std::string marker);
    static void print(const std::string & message);

private:
    enum Type
    {
        _INFO_,
        _WARNING_,
        _ERROR_,
        _DEBUG_,
        _TOKEN_
    };

    static void printLabel(const Type type);
    static int getColorAndLabel(const Type type, std::string & label);
};

#endif // __MESSAGE_HANDLER__
