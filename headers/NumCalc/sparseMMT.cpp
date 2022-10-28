#include"sparse.h"
#include "sparseMMT.h"


/*
sparse matrix multiplication [A*B^T]
1. iterate over all nodes (image) - index i
1.1. getting the i-th row (in our sparse matrix format);
1.2. getting the j-th col of second matrix (which is transpose of initial) 
    => j-th row insted
1.3. calculating the matrix entry A_ij via summation formula
*/

Sparse SparseMMT( Sparse A, Sparse B, int n, int m)
{
    Sparse C;
    for (int i = 0; i < n; i++)
    {
        vector<int> iRowCol;
        vector<double> iRowVal;
        for (int i_2 = 0; i_2 < A.rows.size(); i_2++)
        {
            int alpha = A.rows[i_2];
            if ( alpha == i )
            {
                iRowCol.push_back( A.column[i_2] );
                iRowVal.push_back( A.values[i_2] );
            }
            
        }
        
        for (int j = 0; j < m; j++)
        {
            double C_ij = 0;
            vector<int> jRowCol;
            vector<double> jRowVal;
            for (int j_2 = 0; j_2 < B.rows.size(); j_2++)
            {
                int alpha = B.rows[j_2];
                if ( alpha == j )
                {
                    jRowCol.push_back( B.column[j_2] );
                    jRowVal.push_back( B.values[j_2] );
                }
                
            }
            for (int k_2 = 0; k_2 < iRowCol.size(); k_2++)
            {
                int alpha = iRowCol[k_2];
                for (int k_3 = 0; k_3 < jRowCol.size(); k_3++)
                {
                    int beta = jRowCol[k_3];
                    if ( alpha == beta)
                    {
                        C_ij += iRowVal[k_2]*jRowVal[k_3];
                    }
                        
                }
                    
            }
        if ( C_ij != 0 )
        {
            C.rows.push_back(i);
            C.column.push_back(j);
            C.values.push_back(C_ij);
        }
        
            
        }
        
    }
    return C;
}