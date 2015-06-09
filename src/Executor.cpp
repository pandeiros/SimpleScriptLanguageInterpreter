#include "Executor.h"

#include "Library.h"

void Executor::execute(const std::vector<std::shared_ptr<inter::Function>>& functions)
{
    std::unordered_map<std::string, std::shared_ptr<inter::Function>> definedFunctions;
    for(auto& it: functions)
    {
        std::pair<std::string, std::shared_ptr<inter::Function> > newPair(it->name, it);
        definedFunctions.insert(newPair);
    }

    auto& main = definedFunctions.at("main");

    main->execute(nullptr, definedFunctions, {});

}
