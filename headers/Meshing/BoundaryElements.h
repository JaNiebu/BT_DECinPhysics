#include "../headers.h"
#include "../NumCalc/sparse.h"

vector<int> GetBoundaryEdgesIndices( int Edgesnumber, Sparse Boundary2 );
vector<int> GetBoundaryNodesIndices( int Nodesnumber , vector<array<int,2>> BoundaryEdges );
vector<int> GetBoundaryFacesIndices( vector<array<int,3>> Faces , vector<int> BoundaryNodesIndices );
