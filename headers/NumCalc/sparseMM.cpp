#include "sparse.h"
#include "sparseMM.h"
#include <iostream>


/*
sparse matrix multiplication
1. iterate over all nodes (image) - index i
1.1. getting the i-th row (in our sparse matrix format);
1.2. getting the j-th col of second matrix
1.3. calculating the matrix entry A_ij via summation formula
-> arguments - A: left matrix, B: right matrix, n: rows of A , m: , n_2: dim rows B/cols A
*/


Sparse SparseMM( Sparse A, Sparse B, int n, int m )
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
            vector<int> jColRow;
            vector<double> jColVal;
            for (int j_2 = 0; j_2 < B.column.size(); j_2++)
            {
                int alpha = B.column[j_2];
                if ( alpha == j )
                {
                    jColRow.push_back( B.rows[j_2] );
                    jColVal.push_back( B.values[j_2] );
                }
                
            }
            
            for (int k_2 = 0; k_2 < iRowCol.size(); k_2++)
            {
                int alpha = iRowCol[k_2];
                for (int k_3 = 0; k_3 < jColRow.size(); k_3++)
                {
                    int beta = jColRow[k_3];
                    if ( alpha == beta)
                    {
                        C_ij += iRowVal[k_2]*jColVal[k_3];
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
};


Sparse SparseInvMM( Sparse A, Sparse B, int n, int m)
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
                iRowVal.push_back( 1/A.values[i_2] );
            }
            
        }
        
        for (int j = 0; j < m; j++)
        {
            double C_ij = 0;
            vector<int> jColRow;
            vector<double> jColVal;
            for (int j_2 = 0; j_2 < B.column.size(); j_2++)
            {
                int alpha = B.column[j_2];
                if ( alpha == j )
                {
                    jColRow.push_back( B.rows[j_2] );
                    jColVal.push_back( B.values[j_2] );
                }
                
            }
            for (int k_2 = 0; k_2 < iRowCol.size(); k_2++)
            {
                int alpha = iRowCol[k_2];
                for (int k_3 = 0; k_3 < jColRow.size(); k_3++)
                {
                    int beta = jColRow[k_3];
                    if ( alpha == beta)
                    {
                        C_ij += iRowVal[k_2]*jColVal[k_3];
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
};

vector<double> SparseVecMR( Sparse A , vector<double> x , int n )
{
    //function for Matrix-Vector product of form A*x in Sparse format
    //n is dimension of image i.e. number of rows
    vector<double> y;
    for (int i = 0; i < n; i++)
    {
        vector<int> row_i_columns;
        vector<double> row_i_values;
        for (int j = 0; j < A.rows.size(); j++)
        {
            if ( A.rows[j]==i )
            {
                row_i_columns.push_back( A.column[j] );
                row_i_values.push_back( A.values[j] );
            }
            
        }
        double y_i = 0;
        for (int j = 0; j < row_i_columns.size(); j++)
        {
            int index = row_i_columns[j];
            y_i += row_i_values[j]*x[index];
        }
        y.push_back( y_i );
        
    }
    return y;
    
}