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

#include "Assignable.h"
#include "Assignment.h"
#include "Call.h"
#include "Condition.h"
#include "Expression.h"
#include "FunctionDefinition.h"
#include "IfStatement.h"
#include "LoopJump.h"
#include "Literal.h"
#include "Program.h"
#include "ReturnStatement.h"
#include "StatementBlock.h"
#include "VarDeclaration.h"
#include "Variable.h"
#include "WhileStatement.h"

#endif // __NODE_H__
