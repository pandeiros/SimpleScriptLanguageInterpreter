#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Node.h"

/*
 *
 */
class Matrix : public Node
{
public:
    void pushValue (const double& value)
    {
        this->data.at (currentLevel).push_back (value);
    }
    void nextLevel ()
    {
        this->data.push_back ({});
        this->currentLevel++;
    }
    virtual Type getType ()
    {
        return Node::Type::Matrix;
    }

    unsigned int currentLevel = 0;
    std::vector<std::vector<double>> data;// = {{}};
};

#endif // __MATRIX_H__