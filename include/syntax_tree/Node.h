#ifndef __NODE_H__
#define __NODE_H__

#include <memory>

class Node
{
protected:
    std::weak_ptr<Node> parent;

public:
    enum class Type
    {
        Assignment,
        Call,
        Condition,
        Expression,
        FunDefinition,
        IfStatement,
        LoopJump,
        Matrix,
        Program,
        ReturnStatement,
        StatementBlock,
        VarDeclaration,
        Variable,
        WhileStatement
    };

    virtual Type getType () = 0;
};

typedef std::shared_ptr<Node> NodePtr;

#include "Assignable.h"
#include "Assignment.h"
#include "Call.h"
#include "Condition.h"
#include "Expression.h"
#include "FunDefinition.h"
#include "IfStatement.h"
#include "LoopJump.h"
#include "Matrix.h"
#include "Program.h"
#include "ReturnStatement.h"
#include "StatementBlock.h"
#include "VarDeclaration.h"
#include "Variable.h"
#include "WhileStatement.h"

#endif // __NODE_H__
