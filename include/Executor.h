#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include <unordered_map>
#include <string>
#include "intermediate/Function.h"

class Executor
{
public:
    void execute(const std::vector<std::shared_ptr<inter::Function>>& functions);
};

#endif
