#ifndef sparse
#define sparse
#include "../headers.h"


struct Sparse
{
    vector<double> values;
    vector<int> rows;
    vector<int> column;
};

double FindEntryij( int i , int j , Sparse A );

vector<double> GetRow( int IndexRow , Sparse Matrix , int TopDimension );

#endif