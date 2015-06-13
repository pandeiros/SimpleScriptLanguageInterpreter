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
        INFO,
        WARNING,
        ERROR,
        DEBUG,
        TOKEN
    };

    static void printLabel(const Type type);
};

#endif // __MESSAGE_HANDLER__
