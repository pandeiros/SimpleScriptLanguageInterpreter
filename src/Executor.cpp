#include "Executor.h"

#include "Library.h"

void Executor::execute(const std::vector<std::shared_ptr<inter::Function>>& functions)
{
    // Add defined functions for execution.
    std::unordered_map<std::string, std::shared_ptr<inter::Function>> definedFunctions;
    for(auto & function : functions)
    {
        std::pair<std::string, std::shared_ptr<inter::Function> > newPair(function->name, function);
        definedFunctions.insert(newPair);
    }

    // Find entry point ("program" function) and execute it all!
    auto & entryPoint = definedFunctions.at("program"); 
    entryPoint->execute(nullptr, definedFunctions, {});
}
