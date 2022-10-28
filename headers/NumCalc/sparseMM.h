#ifndef sparseMM
#define sparseMM

#include "sparse.h"

Sparse SparseMM( Sparse A , Sparse B, int n , int m );

Sparse SparseInvMM( Sparse A , Sparse B , int n , int m );

vector<double> SparseVecMR( Sparse A , vector<double> x , int n );

#endif