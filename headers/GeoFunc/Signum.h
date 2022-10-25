
#ifndef Signum
#define Signum
#include "../headers.h"
#include "../Meshing/TriangulationFunc.h"

using namespace std;

int Sgn2D( array<int,3> s_1 , array<int,3> s_2 , vector<array<double,2>> V );

int Sgn1D( array<int,2> e_1 , array<int,2> e_2 , vector<array<double,2>> V );

int StrictOrientationComparison( array<int,2> FirstEdge , array<int,2> SecondEdge );

#endif