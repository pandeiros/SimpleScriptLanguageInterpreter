#ifndef __MESSAGE_HANDLER__
#define __MESSAGE_HANDLER__

/**
 *
 */
class MessageHandler
{
public:
    static void error(const std::string & message);
    static void warning(const std::string & message);
    static void info(const std::string & message);
    static void print(const std::string & message);

private:
    enum Type
    {
        INFO,
        WARNING,
        ERROR
    };

    static void printLabel(const Type type);
};

#endif // __MESSAGE_HANDLER__
