#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <string>

/**
 *
 */
class InputManager
{
public:

    inline void setSourceFile (const std::string sourceFile)
    {
        this->_sourceFile = sourceFile;
    }

private:
    std::string _sourceFile;
};

#endif  // __INPUT_MANAGER_H__
