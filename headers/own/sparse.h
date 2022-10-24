#ifndef sparse
#define sparse
#include <vector>


struct Sparse
{
    std::vector<double> values;
    std::vector<int> rows;
    std::vector<int> column;
};

#endif