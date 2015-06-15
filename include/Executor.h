#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__

#include <unordered_map>
#include <string>
#include "intermediate/Function.h"

/*
 * Simply: executes prepared functions.
 */
class Executor
{
public:
    void execute(const std::vector<std::shared_ptr<inter::Function>> & functions);
};

#endif // __EXECUTOR_H__
