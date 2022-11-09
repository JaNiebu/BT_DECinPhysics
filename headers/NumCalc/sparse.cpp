#include "sparse.h"

double FindEntryij( int i , int j , Sparse A )
{
    double result = 0.;
    for (int k = 0; k < A.column.size(); k++)
    {
        if ( A.rows[k]==i && A.column[k]==j )
        {
            result = A.values[k];
            break;
        }

        
    }
    return result;
    
}