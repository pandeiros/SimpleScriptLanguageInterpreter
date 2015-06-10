#ifndef __INTER_LOOP_JUMP_INSTR_H__
#define __INTER_LOOP_JUMP_INSTR_H__

#include "Instruction.h"
#include "Executable.h"
#include <iostream>

namespace inter
{
    struct LoopJumpInstr: public Instruction
    {
        bool isBreak;

        virtual std::shared_ptr<Literal> execute(
            ScopeInstance * scope,
            std::unordered_map<std::string, std::shared_ptr<Function>> & functions
        )
        {
            auto result = std::make_shared<Literal>();
            result->loopJump = true;
            result->isBreak = this->isBreak;

            return result;
        }
    };
}

#endif // __INTER_LOOP_JUMP_INSTR_H__
