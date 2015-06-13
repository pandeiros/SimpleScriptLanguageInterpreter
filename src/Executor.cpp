#include "Executor.h"

#include "Library.h"

void Executor::execute(const std::vector<std::shared_ptr<inter::Function>>& functions)
{
    // Add defined functions for execution.
    std::unordered_map<std::string, std::shared_ptr<inter::Function>> definedFunctions;
    for (auto & function : functions)
    {
        std::pair<std::string, std::shared_ptr<inter::Function> > newPair(function->_name, function);
        definedFunctions.insert(newPair);
    }

    // Find entry point ("program" function) and execute it all!
    for (auto & function : definedFunctions)
    {
        if (function.second->_isMain)
        {
            auto & entryPoint = definedFunctions.at(function.first);
            entryPoint->execute(nullptr, definedFunctions, {});
            break;
        }
    }
}
