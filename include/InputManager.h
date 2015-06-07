#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <string>
#include <fstream>

/**
 *
 */
class InputManager
{
public:
    InputManager () = default;
    //InputManager (const std::string& file);

    const char nextSign ();
    void rewind ();
    const bool hasFinished () const;
    const unsigned int & getCurrentLineNo () const;
    const unsigned int & getCurrentSignPos () const;
    const std::streampos getCurrentLinePos () const;
    const std::string getLine (const std::streampos & linePos);

    inline void setSourceFile (const std::string sourceFile)
    {
        this->_sourceFile = sourceFile;
    }

private:
    std::ifstream _handler;

    unsigned int _currentLineNo = 1;
    unsigned int _currentSignPos = 0;
    std::streampos _currentLinePos;
    char _previousSign = 0;

    std::string _sourceFile;
};

#endif  // __INPUT_MANAGER_H__
