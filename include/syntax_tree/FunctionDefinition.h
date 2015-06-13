#ifndef __FUNCTION_DEFINITION_H__
#define __FUNCTION_DEFINITION_H__

#include "StatementBlock.h"

namespace syntax
{
    class FunctionDefinition : public Node
    {
    public:
        virtual Type getType()
        {
            return Node::Type::FunctionDefinition;
        }

        void setName(const std::string & name)
        {
            _name = name;
        }

        void setParameters(const std::vector<std::string> & types,
                           const std::vector<std::string> & names)
        {
            _names = names;
            _types = types;
        }

        void setBlock(const std::shared_ptr<StatementBlock> & block)
        {
            _block = block;
        }

        bool _isMain = false;
        std::string _name;
        std::vector<std::string> _types;
        std::vector<std::string> _names;
        std::shared_ptr<StatementBlock> _block;
    };
}

#endif // __FUNCTION_DEFINITION_H__