#ifndef __LOOP_JUMP_H__
#define __LOOP_JUMP_H__

#include "Node.h"

/*
 *
 */
namespace syntax
{
    class LoopJump : public Node
    {
    public:
        void setType(const TokenType& type)
        {
            if (type == TokenType::Break)
            {
                this->isBreak = true;
                return;
            }
            this->isBreak = false;
        }
        virtual Type getType()
        {
            return Node::Type::LoopJump;
        }

        bool isBreak;
    };
}

#endif // __LOOP_JUMP_H__