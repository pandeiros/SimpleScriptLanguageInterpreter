#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Node.h"

/*
 *
 */
namespace syntax
{
    class Matrix : public Node
    {
    public:
        void pushValue(const double& value)
        {
            if (data.size() == 0)
                data.push_back(std::vector<double>());
            this->data.at(currentLevel).push_back(value);
        }
        void nextLevel()
        {
            data.push_back(std::vector<double>());
            this->currentLevel++;
        }
        virtual Type getType()
        {
            return Node::Type::Matrix;
        }

        unsigned int currentLevel = 0;
        std::vector<std::vector<double>> data;// = {{}};
    };
}

#endif // __MATRIX_H__