#include "ReadWriteOp.h"

void WriteSparse( string adress , Sparse A , int n , int m )
{
    ofstream out{ adress };
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            double Aij = FindEntryij( i , j , A );
            out << Aij << " ";
        }
        out << endl;

    }
    
}