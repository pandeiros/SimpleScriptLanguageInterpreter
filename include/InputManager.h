#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <string>
#include <fstream>

/**
 * Reads characters from stream.
 */
class InputManager
{
public:
    InputManager() = default;

    // Input controllers.
    const char nextCharacter(bool isComment = false);
    void rewind();              // TODO REMOVE,  USE BUFFER

    // Getters.
    const bool hasFinished() const;
    const unsigned int & getCurrentLineNo() const;
    const unsigned int & getCurrentSignPos() const;
    const std::streampos getCurrentLinePos() const;

    // Get line, whether character occured.
    const std::string getLine(const std::streampos & linePos);

    // Setter for source file path.
    inline void setSourceFile(const std::string sourceFile)
    {
        this->_sourceFile = sourceFile;
        _handler = std::ifstream(sourceFile);
    }

private:
    // Input file handler
    std::ifstream _handler;

    // Line and position indicators.
    unsigned int _currentLineNo = 1;
    unsigned int _currentSignPos = 0;
    std::streampos _currentLinePos;

    // Signs read from buffer.
    char _previousCharacter = 0;
    char _character = ' ';

    // Path to source file.
    std::string _sourceFile;
};

#endif  // __INPUT_MANAGER_H__
