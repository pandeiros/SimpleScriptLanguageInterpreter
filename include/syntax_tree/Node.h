#ifndef __NODE_H__
#define __NODE_H__

#include <memory>

namespace syntax
{
    class Node
    {
    public:
        enum class Type
        {
            Assignment,
            Bool,
            Call,
            Condition,
            Expression,
            FunctionDefinition,
            IfStatement,
            LoopJump,
            Number,
            Literal,
            Program,
            ReturnStatement,
            StatementBlock,
            String,
            VarDeclaration,
            Variable,
            WhileStatement
        };

        virtual Type getType() = 0;

    protected:
        std::weak_ptr<Node> parent;
    };
}

typedef std::shared_ptr<syntax::Node> NodePtr;

#endif // __NODE_H__
