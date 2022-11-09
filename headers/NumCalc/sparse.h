#ifndef sparse
#define sparse
#include "../headers.h"


struct Sparse
{
    vector<double> values;
    vector<int> rows;
    vector<int> column;
};

int FindEntryij( int i , int j , Sparse A );

#endif