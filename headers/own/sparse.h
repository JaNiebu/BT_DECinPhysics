#ifndef sparse
#define sparse
#include "headers.h"


struct Sparse
{
    std::vector<double> values;
    std::vector<int> rows;
    std::vector<int> column;
};

#endif