#ifndef DiagonalHodge
#define DiagonalHodge
#include "../headers.h"
#include "../GeoFunc/Signum.h"
#include "../Meshing/TriangulationFunc.h"
#include "../GeoFunc/Circumcenter.h"
#include "../NumCalc/sparse.h"


using namespace std;

double TriArea( array<double,2> A , array<double,2> B , array<double,2> C );
double CircDualArea( int v , vector<array<double,2>> V , vector<array<int,3>> F );
double CircDualLength( array<int,2> e , vector<array<double,2>> V , vector<array<int,3>> F );
Sparse DiagHodge0D( vector<array<double,2>> V , vector<array<int,3>> F );
Sparse DiagHodge1D( vector<array<double,2>> V , vector<array<int,2>> E , vector<array<int,3>> F );
Sparse DiagHodge2D( vector<array<double,2>> V , vector<array<int,3>> F );

#endif