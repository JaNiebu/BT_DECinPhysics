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

vector<double> GetRow( int IndexRow , Sparse Matrix , int TopDimension )
{
    vector<double> Row;
    for (int i = 0; i < Matrix.column.size(); i++)
    {
        bool empty = true;
        for (int j = 0; j < TopDimension; j++)
        {
        
            if ( Matrix.rows[i]==IndexRow && Matrix.column[i]==j )
            {
                Row.push_back( Matrix.values[i] );
                empty = false;
                break;
            }
        }
        if ( empty==true )
        {
            Row.push_back( 0. );
        }
        
        
    }
    return Row;
    
}
