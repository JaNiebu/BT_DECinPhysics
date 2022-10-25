#ifndef TriangulationFunc
#define TriangulationFunc
#include "../headers.h"
#include "../GeoFunc/Circumcenter.h"
#include "EdgesGen.h"

using namespace std;

double distance(array<double, 2> x , array<double, 2> y);
double circumradius( array<int,3> t , vector<array<double, 2>> V );
bool Circ( array<int,3> t , array<double, 2> v , vector<array<double, 2>> V );
vector<array<int, 3>> sharet( array<int,3> t, array<int, 2> e , vector< array<int, 3>> T);
vector< array<int, 3>> BowyerWatson( vector<array<double, 2>> &vec );
int Orient2D( array<int, 3> t , vector<array<double, 2>> V);
array<double, 3> BarycentCoord( array<double, 2> v , array<int, 3> t , vector<array<double, 2>> V );
bool InsideTriCheck( array<double, 2> v , array<int, 3> t , vector<array<double, 2>> V );

#endif