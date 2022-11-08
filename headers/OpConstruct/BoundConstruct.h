#ifndef BoundConstruct
#define BoundConstruct

#include "../headers.h"
#include "../NumCalc/sparse.h"
#include "../Meshing/EdgesGen.h"
#include "../GeoFunc/Signum.h"


Sparse ConstructBound1( vector<array<int,2>> E , bool SignChange );
Sparse ConstructBound2( vector<array<int,3>> Faces , vector<array<int,2>> Edges , bool SignChange );

#endif